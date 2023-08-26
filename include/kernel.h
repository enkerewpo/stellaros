//
// Created by wheatfox on 8/25/2023.
//

#ifndef STELLAROS_KERNEL_H
#define STELLAROS_KERNEL_H

#define PERIPHERAL_BASE 0xfe000000
#define CLOCKHZ 1000000

#define i32 int
#define u32 unsigned int
#define i64 long long
#define u64 unsigned long long

void irq_init_vectors();
void irq_enable();
void irq_disable();
void handle_timer_1();

#endif // STELLAROS_KERNEL_H
