#[cfg(target_arch = "aarch64")]
#[path = "_arch/aarch64/timer.rs"]
mod arch_timer;

use core::time::Duration;

pub struct Timer;

static TIMER: Timer = Timer::new();

pub fn timer() -> &'static Timer {
    &TIMER
}

impl Timer {
    pub const fn new() -> Self {
        Self
    }
    pub fn resolution(&self) -> Duration {
        arch_timer::resolution()
    }
    pub fn uptime(&self) -> Duration {
        arch_timer::uptime()
    }
    pub fn spin(&self, d: Duration) {
        arch_timer::spin(d)
    }
}
