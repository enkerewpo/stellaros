#include "interrupt.h"

#define CORE0_TIMER_IRQCNTL ((volatile u32 *)(0x40000040))

const u64 timer_period = CLOCKHZ;
u64 timer1_val = 0;

void timer_init() {
  //  timer1_val = REGS_TIMER->counter_lo;
  //  timer1_val += timer_period;
  //  REGS_TIMER->compare[1] = timer1_val;
  asm volatile("mrs %0, cntvct_el0" : "=r"(timer1_val));
  timer1_val += timer_period;
  asm volatile("msr cntv_cval_el0, %0" ::"r"(timer1_val));
  // set CNTV_CTL to enable timer
  u64 val = 0;
  val |= 1 << 0;
  asm volatile("msr cntv_ctl_el0, %0" ::"r"(val));
  // print freq from CNTFRQ
  u64 cntfrq = 0;
  asm volatile("mrs %0, cntfrq_el0" : "=r"(cntfrq));
  print("CNTFRQ: ");
  print_int(cntfrq);
  print("\n");
}

void handle_timer_1() {
  print("Timer 1 triggered!\n");
  __exit_kernel();
}

void enable_interrupt_controller() {
  // set TIMER_CTNRL1 bit [3:0] to 1
  int val = 0;
  val |= 1 << 3;
  val |= 1 << 2;
  val |= 1 << 1;
  val |= 1 << 0;
  mmio_write(TIMER_CNTRL1, val);
  mmio_write(IRQ0_SET_EN_2, (1 << 0));
  // read back IRQ1_ENABLE0
  int tmp = mmio_read(IRQ0_SET_EN_2);
  print("IRQ0_SET_EN_2: ");
  print_binary(tmp);
  print("\n");
  /* nCNTVIRQ routing to CORE0.*/
  *CORE0_TIMER_IRQCNTL = 1 << 3;
}

void handle_irq() {
  print("IRQ triggered!\n");
  handle_timer_1();
}

void handle_invalid_interrupt(int type, int esr, int elr) {
  print("Invalid interrupt!\n");
  print("Type: ");
  print_int(type);
  print("\n");
  print("ESR: ");
  print_int(esr);
  print("\n");
  print("ELR: ");
  print_int(elr);
  print("\n");
  __exit_kernel();
}