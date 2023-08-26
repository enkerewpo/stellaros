// Created by: wheatfox
// Created on: 2023/8/16
// Description: a kernel of StellarOS on Raspberry Pi 4B

#include "kernel.h"
#include "interrupt.h"
#include "io.h"
#include "version.h"

void __exit_kernel() {
  uart_writeText("[INFO] (__exit_kernel) Exiting StellarOS...\n");
  uart_writeText("[INFO] (__exit_kernel) Bye, see you next time!\n");
  asm volatile("wfi");
}

int init() {
  // init uart
  uart_init();

  uart_writeText("[INFO] (init) Booting StellarOS...(");
  uart_writeText(STELLAROS_VERSION_STRING_FULL);
  uart_writeText(")\n");

  // do init stuff here
  uart_writeText("[INFO] (init) Initializing interrupts...\n");

  // irq
  irq_init_vectors();
  irq_enable();

  // enable irq interrupt controller
  enable_interrupt_controller();

  // timer
  timer_init();

  uart_writeText("[INFO] (init) Interrupts initialized.\n");

  uart_writeText("   _____ _       _ _             ____   _____\n");
  uart_writeText("  / ____| |     | | |           / __ \\ / ____|\n");
  uart_writeText(" | (___ | |_ ___| | | __ _ _ __| |  | | (___\n");
  uart_writeText("  \\___ \\| __/ _ \\ | |/ _` | '__| |  | |\\___ \\\n");
  uart_writeText("  ____) | ||  __/ | | (_| | |  | |__| |____) |\n");
  uart_writeText(" |_____/ \\__\\___|_|_|\\__,_|_|   \\____/|_____/\n");

  uart_writeText("Welcome to StellarOS! This is a IoT operating system written "
                 "by wheatfox(enkerewpo@hotmail.com).\n");

  int f = 1;

  while (1) {
    int cnt_vct = 0;
    int cntv_cval = 0;
    int cntv_ctl = 0;
    asm volatile("mrs %0, cntvct_el0" : "=r"(cnt_vct));
    if (cnt_vct % 50000 == 0) {
      //      uart_writeText("COUNT: ");
      //      uart_writeInt(REGS_TIMER->counter_lo);
      //      uart_writeText(" ");
      //      uart_writeText("COMPARE: ");
      //      uart_writeInt(REGS_TIMER->compare[1]);
      //      uart_writeText(" ");
      //      uart_writeText("CONTROL_STATUS: ");
      //      uart_writeBinary(REGS_TIMER->control_status);
      //      uart_writeText("\n");
      // check CNT_VCT and CNTV_CVAL, CNTV_CTL
      uart_writeText("CNT_VCT: ");
      uart_writeInt(cnt_vct);
      uart_writeText(" ");
      asm volatile("mrs %0, cntv_cval_el0" : "=r"(cntv_cval));
      uart_writeText("CNTV_CVAL: ");
      uart_writeInt(cntv_cval);
      uart_writeText(" ");
      asm volatile("mrs %0, cntv_ctl_el0" : "=r"(cntv_ctl));
      uart_writeText("CNTV_CTL: ");
      uart_writeBinary(cntv_ctl);
      uart_writeText("\n");
    }
    if (f && cntv_cval && cnt_vct >= cntv_cval * 2) {
      uart_writeText("Timer 1 Failed To Trigger a Interrupt!\n");
      f = 0;
    }
    if (cntv_cval && cnt_vct >= cntv_cval * 3) {
      uart_writeText("Quitting loop...\n");
      break;
    }
  }
  // entering shell loop
  //  shell_loop();

  return 0;
}