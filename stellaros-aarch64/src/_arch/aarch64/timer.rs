// This module is for aarch64's timer resgisters and interruptions.
// wheatfox 2023.9.6

use crate::{info, warn};
use aarch64_cpu::{asm::barrier, registers::*};
use core::{
    num::{NonZeroU128, NonZeroU32, NonZeroU64},
    ops::{Add, Div},
    time::Duration,
};
use tock_registers::interfaces::Readable;

const NANOSEC_PER_SEC: NonZeroU64 = NonZeroU64::new(1_000_000_000).unwrap();

#[derive(Copy, Clone, PartialOrd, PartialEq)]
struct TimerCount(u64); // Actually wrap the tuple struct.

/// This is the value of CNTFRQ_EL0 before any Rust code is executed.
/// The value was read from the register in boot.s
#[no_mangle]
static FREQ: NonZeroU32 = NonZeroU32::MIN;

/// Returns the current counter value of the archeticural timer.
fn get_timer_frequency() -> NonZeroU32 {
    unsafe { core::ptr::read_volatile(&FREQ) }
}

impl TimerCount {
    pub const MAX_COUNT: Self = TimerCount(u64::MAX);
}

impl Add for TimerCount {
    type Output = Self;
    fn add(self, o: Self) -> Self {
        TimerCount(self.0.wrapping_add(o.0))
    }
}

impl From<TimerCount> for Duration {
    fn from(cnt: TimerCount) -> Self {
        if cnt.0 == 0u64 {
            return Duration::ZERO;
        }
        let freq = get_timer_frequency().get() as u64;
        let secs = cnt.0 / freq; // rounded
        let r = cnt.0 % freq;
        let nanos = unsafe { r.unchecked_mul(u64::from(NANOSEC_PER_SEC)) } / freq;
        Duration::new(secs, nanos as u32)
    }
}

/// Returns the maximum duration that can be represented.
fn max_duration() -> Duration {
    TimerCount::MAX_COUNT.into()
}

/// Returns the resolution of the timer.
pub fn resolution() -> Duration {
    Duration::from(TimerCount(1))
}

pub fn uptime() -> Duration {
    read_cntpct().into()
}

pub fn spin(d: Duration) {
    let start = read_cntpct();
    let delta: TimerCount = match d.try_into() {
        Ok(d) => d,
        Err(msg) => {
            warn!("spin error: {}", msg);
            return;
        }
    };
    let end = start + delta;
    while read_cntpct() < end {}
}

impl TryFrom<Duration> for TimerCount {
    type Error = &'static str;
    fn try_from(d: Duration) -> Result<Self, Self::Error> {
        if d < resolution() {
            return Ok(TimerCount(0));
        }
        if d > max_duration() {
            return Err("Duration too long for conversion!");
        }
        let freq = u32::from(get_timer_frequency()) as u128;
        let duration = d.as_nanos() as u128;
        let cnt = unsafe { duration.unchecked_mul(freq) } / NonZeroU128::from(NANOSEC_PER_SEC);
        Ok(TimerCount(cnt as u64))
    }
}

/// Returns the current counter value of the archeticural timer.
/// CNTPCT -> physical timer count value register
#[inline(always)]
fn read_cntpct() -> TimerCount {
    barrier::isb(barrier::SY); // Prevent that the counter is read ahead of time due to out-of-order
                               // execution.
    let cnt = CNTPCT_EL0.get();
    TimerCount(cnt)
}
