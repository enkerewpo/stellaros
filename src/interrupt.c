#include "interrupt.h"
#include "io.h"
#include "kernel.h"

const u64 timer_period = CLOCKHZ;
u64 timer1_val = 0;

void timer_init() {
  timer1_val = REGS_TIMER->counter_lo;
  timer1_val += timer_period;
  REGS_TIMER->compare[1] = timer1_val;
}

void handle_timer_1() {
  uart_writeText("Timer 1 triggered!\n");
  __exit_kernel();
}

void enable_interrupt_controller() {
  // set LOCAL_TIMER_CONTROL bit TIMER_IRQ_EN[29] to 1
  // set LOCAL_TIMER_CONTROL bit TIMER_EN[28] to 1
  // set LOCAL_TIMER_CONTROL TIMER_TIMEOUT[27:0] to 5000
  int val = 0;
  val |= 1 << 29;
  val |= 1 << 28;
  val |= 5000;
  mmio_write(LOCAL_TIMER_CONTROL, val);
  // set TIMER_CTNRL1 bit [3:0] to 1
  val = 0;
  val |= 1 << 3;
  val |= 1 << 2;
  val |= 1 << 1;
  val |= 1 << 0;
  mmio_write(TIMER_CNTRL1, val);
  // set IRQ1_ENABLE0 to full 1
  val = 0xffffffff;
  mmio_write(IRQ0_SET_EN_0, val);
  mmio_write(IRQ0_SET_EN_1, val);
  mmio_write(IRQ0_SET_EN_2, val);
  mmio_write(IRQ1_SET_EN_0, val);
  mmio_write(IRQ1_SET_EN_1, val);
  mmio_write(IRQ1_SET_EN_2, val);
}

void handle_irq() {
  uart_writeText("IRQ triggered!\n");
  handle_timer_1();
}

void err_print() { uart_writeText("[ERROR] Something went wrong!\n"); }