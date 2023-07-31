#![no_std]
#![no_main]
#![feature(custom_test_frameworks)]
#![test_runner(stellaros::test_runner)]
#![reexport_test_harness_main = "test_main"]

use core::panic::PanicInfo;
use stellaros::{println};
use stellaros::vga_buffer::{info};

#[no_mangle]
pub extern "C" fn _start() -> ! {
  info("StellarOS v0.1.0, wheatfox, 2023");
  stellaros::init();
  #[cfg(test)]
  test_main();
  stellaros::hlt_loop();
}

/// This function is called on panic.
#[cfg(not(test))]
#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
  println!("{}", info);
  stellaros::hlt_loop();
}

#[cfg(test)]
#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
  stellaros::test_panic_handler(info)
}