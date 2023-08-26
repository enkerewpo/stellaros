//
// Created by wheatfox on 8/25/2023.
//

#ifndef STELLAROS_KERNEL_H
#define STELLAROS_KERNEL_H

#define PERIPHERAL_BASE 0xfe000000
#define CLOCKHZ 1000000

struct timer_regs {
  volatile unsigned int control_status;
  volatile unsigned int counter_lo;
  volatile unsigned int counter_hi;
  volatile unsigned int compare[4];
};

#define REGS_TIMER ((struct timer_regs *)(PERIPHERAL_BASE + 0x00003000))

enum vc_irqs {
  SYSTEM_TIMER_IRQ_0 = 1,
  SYSTEM_TIMER_IRQ_1 = 2,
  SYSTEM_TIMER_IRQ_2 = 4,
  SYSTEM_TIMER_IRQ_3 = 8,
  AUX_IRQ = (1 << 29)
};

#define i32 int
#define u32 unsigned int
#define i64 long long
#define u64 unsigned long long

void irq_init_vectors();
void irq_enable();
void irq_disable();
void handle_timer_1();

#endif // STELLAROS_KERNEL_H
