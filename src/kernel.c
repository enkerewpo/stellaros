// Created by: wheatfox
// Created on: 2023/8/16
// Description: a kernel of StellarOS on Raspberry Pi 4B

#include "kernel.h"
#include "io.h"
#include "version.h"

// TIMER FUNCTIONS

const unsigned int timer_period = CLOCKHZ / 5;
unsigned int timer1_val = 0;

void timer_init() {
  timer1_val = REGS_TIMER->counter_lo;
  timer1_val += timer_period;
  REGS_TIMER->compare[1] = timer1_val;
}

void handle_timer_1() {
  timer1_val += timer_period;
  REGS_TIMER->compare[1] = timer1_val;
  REGS_TIMER->control_status |= SYS_TIMER_IRQ_1;
  uart_writeText("Timer 1 interrupt on hardware!\n");
}

int init() {

  // init uart
  uart_init();

  uart_writeText("Booting StellarOS...(");
  uart_writeText(STELLAROS_VERSION_STRING_FULL);
  uart_writeText(")\n");

  // do init stuff here
  uart_writeText("Initializing interrupts... ");
  irq_init_vectors();
  enable_interrupt_controller();
  irq_enable();
  timer_init();
  uart_writeText("done.\n");

  uart_writeText("Welcome to StellarOS!\n");
  while (1) {
    // do nothing now
  }
  return 0;
}