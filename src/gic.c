//
// Created by wheatfox on 8/26/2023.
// https://github.com/s-matyukevich/raspberry-pi-os/issues/237
//
#include "gic.h"
#include "io.h"
#include "kernel.h"

void enable_interrupt(unsigned int irq) {
  uart_writeText("Enabling interrupt: ");
  uart_writeInt(irq);
  uart_writeText("\n");
  unsigned int n = irq / 32;
  unsigned int offset = irq % 32;
  unsigned int enableRegister = GICD_ENABLE_IRQ_BASE + (4 * n);
  uart_writeText("Enable Register: ");
  uart_writeHex(enableRegister);
  uart_writeText("\n");
  mmio_write(enableRegister, 1 << offset);
}

void assign_target(unsigned int irq, unsigned int cpu) {
  unsigned int n = irq / 4;
  unsigned int targetRegister = GIC_IRQ_TARGET_BASE + (4 * n);
  // Currently we only enter the target CPU 0
  mmio_write(targetRegister, mmio_read(targetRegister) | (1 << 8));
}

void show_invalid_entry_message(int type, unsigned long esr,
                                unsigned long address) {
  uart_writeText("Invalid ");
  if (type == 0) {
    uart_writeText("SYNC");
  } else if (type == 1) {
    uart_writeText("IRQ");
  } else if (type == 2) {
    uart_writeText("FIQ");
  } else if (type == 3) {
    uart_writeText("SError");
  }
}

void enable_interrupt_controller() {
  assign_target(SYSTEM_TIMER_IRQ_0, 0);
  enable_interrupt(SYSTEM_TIMER_IRQ_0);
}

void handle_irq(void) {
  //  unsigned int irq_ack_reg = get32(GICC_IAR);
  uart_writeText("Handling IRQ...\n");
  unsigned int irq_ack_reg = mmio_read(GICC_IAR);
  unsigned int irq = irq_ack_reg & 0x2FF;
  switch (irq) {
  case (SYSTEM_TIMER_IRQ_0):
    handle_timer_1();
    //    put32(GICC_EOIR, irq_ack_reg);
    mmio_write(GICC_EOIR, irq_ack_reg);
    break;
  default:
    uart_writeText("Unknown pending irq: ");
    uart_writeInt(irq);
    uart_writeText("\n");
    break;
  }
}

void err_print() {
  uart_writeText("Error: ");
  int err = mmio_read(GICC_IAR);
  uart_writeInt(err);
  uart_writeText("\n");
}