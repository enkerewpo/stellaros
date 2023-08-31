#include "interrupt.h"

#define CORE0_TIMER_IRQCNTL ((volatile u32 *)(0x40000040))

const u64 timer_period = CLOCKHZ;
u64 timer1_val = 0;

void timer_init() {
  //  timer1_val = REGS_TIMER->counter_lo;
  //  timer1_val += timer_period;
  //  REGS_TIMER->compare[1] = timer1_val;
  asm volatile("mrs %0, cntpct_el0" : "=r"(timer1_val));
  timer1_val += timer_period;
  asm volatile("msr cntp_cval_el0, %0" ::"r"(timer1_val));
  // set CNTP_CTL to enable timer
  u64 val = 0;
  val |= 1 << 0;
  asm volatile("msr cntp_ctl_el0, %0" ::"r"(val));
  // print freq from CNTFRQ
  u64 cntfrq = 0;
  asm volatile("mrs %0, cntfrq_el0" : "=r"(cntfrq));
  print("CNTFRQ: ");
  print_int(cntfrq);
  print("\n");
}

void handle_timer_1() {
  print("Timer 1 triggered!");
  print(" Current timer value: ");
  u64 cnt = 0;
  asm volatile("mrs %0, cntpct_el0" : "=r"(cnt));
  print_int(cnt);
  print(" Current Compare value: ");
  asm volatile("mrs %0, cntp_cval_el0" : "=r"(cnt));
  print_int(cnt);
  print("\n");
  // reset timer
  timer1_val += timer_period;
  asm volatile("msr cntp_cval_el0, %0" ::"r"(timer1_val));
  // clean cntps_ctl_el1's ISTATUS
  u64 val = 0;
  asm volatile("mrs %0, cntp_ctl_el0" : "=r"(val));
  val &= ~(1 << 2);
  asm volatile("msr cntp_ctl_el0, %0" ::"r"(val));
}

void enable_irq_by_id(int id) {
  // enable irq by id
  int n = id / 32;
  int addr = GICD_ISENABLERn + n * 4;
  int bit = id % 32;
  u32 val = 1 << bit;
  mmio_write(addr, val);
}

void enable_interrupt_controller() {
  // using GICv2
  mmio_write(GICD_CTLR, 1);
  print("GICD_CTLR: ");
  print_binary(mmio_read(GICD_CTLR));
  print("\n");
  enable_irq_by_id(31);
}

void handle_irq() {
  print("IRQ triggered!\n");
  handle_timer_1();
}

void handle_invalid_interrupt(int type, int esr, int elr) {
  print("Not handled interrupt!\t");
  print("Type:\t");
  print_int(type);
  print(",\t");
  print("ESR:\t");
  print_int(esr);
  print(",\t");
  print("ELR:\t");
  print_int(elr);
  print("\n");
  //  __exit_kernel();
}