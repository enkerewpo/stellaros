#ifndef __STELLAROS_IO_H__
#define __STELLAROS_IO_H__

void uart_init();
void uart_writeText(char *buffer);
void uart_writeInt(int value);

#endif // __STELLAROS_IO_H__