//
// Created by wheatfox on 8/26/2023.
//

#ifndef STELLAROS_INTERRUPT_H
#define STELLAROS_INTERRUPT_H

#include "io_virt.h"
#include "kernel.h"

// #define PERIPHERAL_BASE 0xfe000000 // raspi4b's peripheral base
#define PERIPHERAL_BASE 0x08000000 // qemu's peripheral base for virt
#define CLOCKHZ 1000000

struct timer_regs {
  volatile unsigned int control_status;
  volatile unsigned int counter_lo;
  volatile unsigned int counter_hi;
  volatile unsigned int compare[4];
};

#define REGS_TIMER ((struct timer_regs *)(PERIPHERAL_BASE + 0x00003000))

#define ARM_LOCAL_BASE 0xff800000 // raspi4b's arm local base

enum ARM_LOCAL {
  ARM_CONTROL = ARM_LOCAL_BASE,
  CORE_IRQ_CONTROL = ARM_LOCAL_BASE + 0x0c,
  PMU_CONTROL_SET = ARM_LOCAL_BASE + 0x10,
  PMU_CONTROL_CLR = ARM_LOCAL_BASE + 0x14,
  PERI_IRQ_ROUTE0 = ARM_LOCAL_BASE + 0x24,
  AXI_QUIET_TIME = ARM_LOCAL_BASE + 0x30,
  LOCAL_TIMER_CONTROL = ARM_LOCAL_BASE + 0x34,
  LOCAL_TIMER_IRQ = ARM_LOCAL_BASE + 0x38,
  TIMER_CNTRL0 = ARM_LOCAL_BASE + 0x40,
  TIMER_CNTRL1 = ARM_LOCAL_BASE + 0x44,
  TIMER_CNTRL2 = ARM_LOCAL_BASE + 0x48,
  TIMER_CNTRL3 = ARM_LOCAL_BASE + 0x4c,
  IRQ_SOURCE0 = ARM_LOCAL_BASE + 0x60,
  IRQ_SOURCE1 = ARM_LOCAL_BASE + 0x64,
  IRQ_SOURCE2 = ARM_LOCAL_BASE + 0x68,
  IRQ_SOURCE3 = ARM_LOCAL_BASE + 0x6c,
  FIQ_SOURCE0 = ARM_LOCAL_BASE + 0x70,
  FIQ_SOURCE1 = ARM_LOCAL_BASE + 0x74,
  FIQ_SOURCE2 = ARM_LOCAL_BASE + 0x78,
  FIQ_SOURCE3 = ARM_LOCAL_BASE + 0x7c,
};

#define ARMC_BASE 0x7e00b000 // raspi4b's arm core base

enum ARMC {
  IRQ0_PENDING0 = ARMC_BASE + 0x200,
  IRQ0_PENDING1 = ARMC_BASE + 0x204,
  IRQ0_PENDING2 = ARMC_BASE + 0x208,
  IRQ0_SET_EN_0 = ARMC_BASE + 0x210,
  IRQ0_SET_EN_1 = ARMC_BASE + 0x214,
  IRQ0_SET_EN_2 = ARMC_BASE + 0x218,
  IRQ0_CLR_EN_0 = ARMC_BASE + 0x220,
  IRQ0_CLR_EN_1 = ARMC_BASE + 0x224,
  IRQ0_CLR_EN_2 = ARMC_BASE + 0x228,

  IRQ_STATUS0 = ARMC_BASE + 0x230,
  IRQ_STATUS1 = ARMC_BASE + 0x234,
  IRQ_STATUS2 = ARMC_BASE + 0x238,

  IRQ1_PENDING0 = ARMC_BASE + 0x240,
  IRQ1_PENDING1 = ARMC_BASE + 0x244,
  IRQ1_PENDING2 = ARMC_BASE + 0x248,
  IRQ1_SET_EN_0 = ARMC_BASE + 0x250,
  IRQ1_SET_EN_1 = ARMC_BASE + 0x254,
  IRQ1_SET_EN_2 = ARMC_BASE + 0x258,
  IRQ1_CLR_EN_0 = ARMC_BASE + 0x260,
  IRQ1_CLR_EN_1 = ARMC_BASE + 0x264,
  IRQ1_CLR_EN_2 = ARMC_BASE + 0x268,

  IRQ2_PENDING0 = ARMC_BASE + 0x280,
  IRQ2_PENDING1 = ARMC_BASE + 0x284,
  IRQ2_PENDING2 = ARMC_BASE + 0x288,
  IRQ2_SET_EN_0 = ARMC_BASE + 0x290,
  IRQ2_SET_EN_1 = ARMC_BASE + 0x294,
  IRQ2_SET_EN_2 = ARMC_BASE + 0x298,
  IRQ2_CLR_EN_0 = ARMC_BASE + 0x2a0,
  IRQ2_CLR_EN_1 = ARMC_BASE + 0x2a4,
  IRQ2_CLR_EN_2 = ARMC_BASE + 0x2a8,

  IRQ3_PENDING0 = ARMC_BASE + 0x2c0,
  IRQ3_PENDING1 = ARMC_BASE + 0x2c4,
  IRQ3_PENDING2 = ARMC_BASE + 0x2c8,
  IRQ3_SET_EN_0 = ARMC_BASE + 0x2d0,
  IRQ3_SET_EN_1 = ARMC_BASE + 0x2d4,
  IRQ3_SET_EN_2 = ARMC_BASE + 0x2d8,
  IRQ3_CLR_EN_0 = ARMC_BASE + 0x2e0,
  IRQ3_CLR_EN_1 = ARMC_BASE + 0x2e4,
  IRQ3_CLR_EN_2 = ARMC_BASE + 0x2e8,
};

// this is for qemu virt's GICv2
#define GICv2_BASE 0x8000000
#define GICv2_DIST_BASE 0x8000000
#define GICv2_CPU_BASE 0x8010000
enum GICv2 {
  GICD_CTLR = GICv2_DIST_BASE + 0x000,  // Distributor Control Register
  GICD_TYPER = GICv2_DIST_BASE + 0x004, // Interrupt Controller Type Register
  GICD_IIDR = GICv2_DIST_BASE + 0x008, // Distributor Implementer Identification
                                       // Register
  GICD_IGROUPRn = GICv2_DIST_BASE + 0x080,   // Interrupt Group Registers
  GICD_ISENABLERn = GICv2_DIST_BASE + 0x100, // Interrupt Set-Enable Registers
  GICD_ICENABLERn = GICv2_DIST_BASE + 0x180, // Interrupt Clear-Enable Registers
  GICD_ISPENDRn = GICv2_DIST_BASE + 0x200,   // Interrupt Set-Pending Registers
  GICD_ICPENDRn = GICv2_DIST_BASE + 0x280, // Interrupt Clear-Pending Registers
  GICD_ISACTIVERn = GICv2_DIST_BASE + 0x300, // Interrupt Set-Active Registers
  GICD_ICACTIVERn = GICv2_DIST_BASE + 0x380, // Interrupt Clear-Active Registers
  GICD_IPRIORITYRn = GICv2_DIST_BASE + 0x400, // Interrupt Priority Registers
  GICD_ITARGETSRn =
      GICv2_DIST_BASE + 0x800, // Interrupt Processor Targets Registers
  GICD_ICFGRn = GICv2_DIST_BASE + 0xc00, // Interrupt Configuration Registers
  GICD_NSACRn = GICv2_DIST_BASE + 0xe00, // Non-secure Access Control Registers
  GICD_SGIR = GICv2_DIST_BASE + 0xf00, // Software Generated Interrupt Register
  GICD_CPENDSGIRn = GICv2_DIST_BASE + 0xf10, // SGI Clear-Pending Registers
  GICD_SPENDSGIRn = GICv2_DIST_BASE + 0xf20, // SGI Set-Pending Registers

  GICC_CTLR = GICv2_CPU_BASE + 0x0000,   // CPU Interface Control Register
  GICC_PMR = GICv2_CPU_BASE + 0x0004,    // Interrupt Priority Mask Register
  GICC_BPR = GICv2_CPU_BASE + 0x0008,    // Binary Point Register
  GICC_IAR = GICv2_CPU_BASE + 0x000c,    // Interrupt Acknowledge Register
  GICC_EOIR = GICv2_CPU_BASE + 0x0010,   // End of Interrupt Register
  GICC_RPR = GICv2_CPU_BASE + 0x0014,    // Running Priority Register
  GICC_HPPIR = GICv2_CPU_BASE + 0x0018,  // Highest Priority Pending Interrupt
                                         // Register
  GICC_ABPR = GICv2_CPU_BASE + 0x001c,   // Aliased Binary Point Register
  GICC_AIAR = GICv2_CPU_BASE + 0x0020,   // Aliased Interrupt Acknowledge
                                         // Register
  GICC_AEOIR = GICv2_CPU_BASE + 0x0024,  // Aliased End of Interrupt Register
  GICC_AHPPIR = GICv2_CPU_BASE + 0x0028, // Aliased Highest Priority Pending
                                         // Interrupt Register
  GICC_APRn = GICv2_CPU_BASE + 0x00d0,   // Active Priorities Registers
  GICC_NSAPRn = GICv2_CPU_BASE + 0x00e0, // Non-secure Active Priorities
                                         // Registers
  GICC_IIDR = GICv2_CPU_BASE + 0x00fc, // CPU Interface Identification Register
  GICC_DIR = GICv2_CPU_BASE + 0x1000,  // Deactivate Interrupt Register
};

void irq_init_vectors();
void irq_enable();
void irq_disable();
void timer_init();
void handle_timer_1();
void enable_interrupt_controller();

#endif // STELLAROS_INTERRUPT_H
