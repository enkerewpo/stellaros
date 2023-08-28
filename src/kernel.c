// Created by: wheatfox
// Created on: 2023/8/16
// Description: a kernel of StellarOS on aaarch64

#include "kernel.h"
#include "interrupt.h"
#include "io_virt.h"
#include "version.h"

void __exit_kernel() {
  print("[INFO] (__exit_kernel) Exiting StellarOS...\n");
  print("[INFO] (__exit_kernel) Bye, see you next time!\n");
  asm volatile("wfi");
}

int init() {

  print("[INFO] (init) Booting StellarOS...(");
  print(STELLAROS_VERSION_STRING_FULL);
  print(")\n");

  // do init stuff here
  print("[INFO] (init) Initializing interrupts...\n");

  // irq
  irq_init_vectors();
  irq_enable();

  // enable irq interrupt controller
  enable_interrupt_controller();

  // timer
  timer_init();

  print("[INFO] (init) Interrupts initialized.\n");

  print("   _____ _       _ _             ____   _____\n");
  print("  / ____| |     | | |           / __ \\ / ____|\n");
  print(" | (___ | |_ ___| | | __ _ _ __| |  | | (___\n");
  print("  \\___ \\| __/ _ \\ | |/ _` | '__| |  | |\\___ \\\n");
  print("  ____) | ||  __/ | | (_| | |  | |__| |____) |\n");
  print(" |_____/ \\__\\___|_|_|\\__,_|_|   \\____/|_____/\n");

  print("Welcome to StellarOS! This is a IoT operating system written "
        "by wheatfox(enkerewpo@hotmail.com).\n");

  // entering shell loop
  shell_loop();

  return 0;
}