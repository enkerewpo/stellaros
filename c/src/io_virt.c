#if 1

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
  print("mmio_write: ADDR=");
  print_hex(reg);
  print(" <- DATA=");
  print_hex(val);
  print("\n");
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

void print_hex(unsigned int value) {
  print("0x");
  if (value == 0) {
    putchar('0');
    return;
  }
  char buffer[8];
  int i = 0;
  while (value > 0) {
    int digit = value % 16;
    if (digit < 10) {
      buffer[i] = '0' + digit;
    } else {
      buffer[i] = 'a' + digit - 10;
    }
    value /= 16;
    i++;
  }
  while (i > 0) {
    i--;
    putchar(buffer[i]);
  }
}

// TODO: not working
unsigned char getchar() {
  while (1) {
    if ((*UART & 0x01) == 0x01) {
      return *UART;
    }
  }
}

#endif