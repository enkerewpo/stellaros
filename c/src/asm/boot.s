#include "sysregs.h"

#define LOCAL_CONTROL   0xff800000
#define LOCAL_PRESCALER 0xff800008
#define OSC_FREQ        54000000
#define MAIN_STACK      0x400000

.section ".text.boot"
.global _start

_start:
    mrs x1, mpidr_el1
    and x1, x1, #3
    cbz x1, 2f // if cpu0, jump to 2, else wait for event
1:
    wfe
    b 1b
2:
    ldr x1, =_start
    mov sp, x1 // set stack pointer to _start
    ldr x1, =__bss_start
    ldr w2, =__bss_size
3:
    cbz w2, 4f
    str xzr, [x1], #8
    sub w2, w2, #1
    cbnz w2, 3b
4:
    bl init
    b 1b

.globl exit_kernel
exit_kernel:
    bl __exit_kernel
