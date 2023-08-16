// Created by: wheatfox
// Created on: 2023/8/16
// Description: a kernel for StellarOS on Raspberry Pi 4B

#include "io.h"

void main() {
  uart_init();
  uart_writeText("Hello world!\n");
  while (1) {
    // do nothing
  }
}