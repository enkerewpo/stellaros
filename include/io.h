#ifndef __STELLAROS_IO_H__
#define __STELLAROS_IO_H__

void uart_init();
void uart_writeText(char *buffer);
void uart_writeInt(unsigned int value);
void uart_writeBinary(unsigned int value);
void uart_writeHex(unsigned int value);
void mmio_write(long reg, unsigned int val);
unsigned int mmio_read(long reg);

#endif // __STELLAROS_IO_H__