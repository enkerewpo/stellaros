#ifndef __STELLAROS_IO_H__
#define __STELLAROS_IO_H__

void uart_init();
void uart_writeText(char *buffer);
void uart_writeInt(int value);
void uart_writeBinary(int value);
void uart_writeHex(int value);
void mmio_write(long reg, unsigned int val);
unsigned int mmio_read(long reg);

#endif // __STELLAROS_IO_H__