#ifndef __STELLAROS_IO_VIRT_H__
#define __STELLAROS_IO_VIRT_H__

#include <stdint.h>

void putchar(char c);
void print(const char *s);
void print_int(unsigned int value);
void print_binary(unsigned int value);
unsigned char getchar();
void mmio_write(long reg, unsigned int val);
unsigned int mmio_read(long reg);

#endif // __STELLAROS_IO_VIRT_H__