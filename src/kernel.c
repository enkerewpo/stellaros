// Created by: wheatfox
// Created on: 2023/8/16
// Description: a kernel of StellarOS on Raspberry Pi 4B

#include "kernel.h"
#include "gic.h"
#include "io.h"
#include "version.h"

// TIMER FUNCTIONS

const u64 timer_period = CLOCKHZ / 10;
u64 timer1_val = 0;

void timer_init() {
  asm volatile("msr cntv_ctl_el0, %0" ::"r"(1ull)); // enable timer
  u64 next = timer1_val + timer_period;
  asm volatile("msr cntv_cval_el0, %0" ::"r"(next)); // compare value
}

void handle_timer_1() {
  uart_writeText("Timer 1 triggered!\n");
  // update timer1_val
  asm volatile("mrs %0, cntv_cval_el0" : "=r"(timer1_val));
  u64 next = timer1_val + timer_period;
  asm volatile("msr cntv_cval_el0, %0" ::"r"(next)); // compare value
}

int init() {
  // init uart
  uart_init();

  uart_writeText("Booting StellarOS...(");
  uart_writeText(STELLAROS_VERSION_STRING_FULL);
  uart_writeText(")\n");

  // do init stuff here
  uart_writeText("Initializing interrupts...\n");

  // irq
  irq_init_vectors();
  enable_interrupt_controller();
  irq_enable();

  // timer
  timer_init();

  uart_writeText("Interrupts initialized.\n");
  uart_writeText("Welcome to StellarOS!\n");

  // entering shell loop
  shell_loop();

  return 0;
}

void __exit_kernel() {
  uart_writeText("Exiting StellarOS...\n");
  uart_writeText("Bye!\n");
  asm volatile("wfi");
}