#![no_std]
#![no_main]
#![feature(custom_test_frameworks)]
#![test_runner(stellaros::test_runner)]
#![reexport_test_harness_main = "test_main"]

use core::panic::PanicInfo;
use stellaros::{memory, println};
use stellaros::vga_buffer::{info};
use bootloader::{BootInfo, entry_point};
use x86_64::structures::paging::Translate;
use x86_64::VirtAddr;

entry_point!(kernel_main);

#[no_mangle]
fn kernel_main(boot_info: &'static BootInfo) -> ! {
  info("StellarOS v0.1.0, wheatfox, 2023");
  stellaros::init();
  let phys_mem_offset = VirtAddr::new(boot_info.physical_memory_offset);
  let mapper = unsafe { memory::init(phys_mem_offset) };  let addresses = [
    // the identity-mapped vga buffer page
    0xb8000,
    // some code page
    0x201008,
    // some stack page
    0x0100_0020_1a10,
    // virtual address mapped to physical address 0
    boot_info.physical_memory_offset,
  ];
  for &address in &addresses {
    let virt = VirtAddr::new(address);
    // new: use the `mapper.translate_addr` method
    let phys = mapper.translate_addr(virt);
    println!("{:?} -> {:?}", virt, phys.unwrap());
  }
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