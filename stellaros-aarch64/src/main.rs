// These are all the features that are used in the project
#![allow(clippy::upper_case_acronyms)]
#![feature(asm_const)]
#![feature(format_args_nl)]
#![feature(const_option)]
#![feature(panic_info_message)]
#![feature(trait_alias)]
#![feature(unchecked_math)]
#![no_main]
#![no_std]

// modules
mod bsp;
mod console;
mod cpu;
mod driver;
mod panic_wait;
mod print;
mod shell;
mod synchronization;
mod timer;

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

    info!(
        "(kernel_main): {} version {}",
        env!("CARGO_PKG_NAME"),
        env!("CARGO_PKG_VERSION")
    );
    info!("(kernel_main): Booting on: {}", bsp::board_name());
    info!("(kernel_main): Drivers loaded:");
    driver::driver_manager().enumerate();

    info!("(kernel_main): Entering shell ...");
    console().clear_rx();
    shell::main_loop()
}
