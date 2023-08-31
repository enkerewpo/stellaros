#ifndef __STELLAROS_IO_H__
#define __STELLAROS_IO_H__

#include "kernel.h"

#if 0

void uart_init();
void uart_writeText(char *buffer);
void uart_writeInt(unsigned int value);
void uart_writeBinary(unsigned int value);
void uart_writeHex(unsigned int value);
void uart_writeChar(unsigned char value);
unsigned char uart_readByte();
void putchar(char c);
void print(const char *s);
void print_int(unsigned int value);
void print_binary(unsigned int value);
void print_hex(unsigned int value);
unsigned char getchar();
void mmio_write(long reg, unsigned int val);
unsigned int mmio_read(long reg);

#endif // IO_RASPI4B

#endif // __STELLAROS_IO_H__