#include "io_virt.h"

volatile uint8_t *UART = (uint8_t *)0x09000000; // UART0 on QEMU virt

void putchar(char c) { *UART = c; }

void print(const char *s) {
  while (*s != '\0') {
    putchar(*s);
    s++;
  }
}

void mmio_write(long reg, unsigned int val) {
  *(volatile unsigned int *)reg = val;
}
unsigned int mmio_read(long reg) { return *(volatile unsigned int *)reg; }

void print_int(unsigned int value) {
  if (value == 0) {
    putchar('0');
    return;
  }
  char buffer[10];
  int i = 0;
  while (value > 0) {
    buffer[i] = '0' + (value % 10);
    value /= 10;
    i++;
  }
  while (i > 0) {
    i--;
    putchar(buffer[i]);
  }
}

void print_binary(unsigned int value) {
  if (value == 0) {
    putchar('0');
    return;
  }
  char buffer[32];
  int i = 0;
  while (value > 0) {
    buffer[i] = '0' + (value % 2);
    value /= 2;
    i++;
  }
  while (i > 0) {
    i--;
    putchar(buffer[i]);
  }
}

unsigned char getchar() {
  while (1) {
    if ((*UART & 0x01) == 0x01) {
      return *UART;
    }
  }
}