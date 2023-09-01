#![allow(clippy::upper_case_acronyms)]
#![feature(asm_const)]
#![feature(format_args_nl)]
#![feature(panic_info_message)]
#![feature(trait_alias)]
#![no_main]
#![no_std]

mod bsp;
mod console;
mod cpu;
mod driver;
mod panic_wait;
mod print;
mod synchronization;

/// Early init code.
///
/// # Safety
///
/// - Only a single core must be active and running this function.
/// - The init calls in this function must appear in the correct order.
unsafe fn kernel_init() -> ! {
    // Initialize the BSP driver subsystem.
    if let Err(x) = bsp::driver::init() {
        panic!("Error initializing BSP driver subsystem: {}", x);
    }

    // Initialize all device drivers.
    driver::driver_manager().init_drivers();
    // println! is usable from here on.

    // Transition from unsafe to safe.
    kernel_main()
}

/// The main function running after the early init.
fn kernel_main() -> ! {
    use console::console;

    print!("   _____ _       _ _             ____   _____\n");
    print!("  / ____| |     | | |           / __ \\ / ____|\n");
    print!(" | (___ | |_ ___| | | __ _ _ __| |  | | (___\n");
    print!("  \\___ \\| __/ _ \\ | |/ _` | '__| |  | |\\___ \\\n");
    print!("  ____) | ||  __/ | | (_| | |  | |__| |____) |\n");
    print!(" |_____/ \\__\\___|_|_|\\__,_|_|   \\____/|_____/\n");

    println!(
        "[INFO] (kernel_main): {} version {}",
        env!("CARGO_PKG_NAME"),
        env!("CARGO_PKG_VERSION")
    );
    println!("[INFO] (kernel_main): Booting on: {}", bsp::board_name());

    println!("[INFO] (kernel_main): Drivers loaded:");
    driver::driver_manager().enumerate();

    println!(
        "[INFO] (kernel_main): Chars written: {}",
        console().chars_written()
    );
    println!("[INFO] (kernel_main): Echoing input now");

    // Discard any spurious received characters before going into echo mode.
    console().clear_rx();
    loop {
        let c = console().read_char();
        console().write_char(c);
    }
}
