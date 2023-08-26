#include "shell.h"
#include "io.h"
#include "version.h"

char buffer[2048]; // for shell input

void print_prefix() { uart_writeText("StellarOS> "); }

int so_strcmp(char *a, char *b) {
  int i = 0;
  while (1) {
    if (a[i] == 0 && b[i] == 0)
      return 0;
    if (a[i] == 0)
      return -1;
    if (b[i] == 0)
      return 1;
    if (a[i] != b[i])
      return a[i] - b[i];
    i++;
  }
}

void so_memset(char *buffer, char value, int size) {
  for (int i = 0; i < size; i++) {
    buffer[i] = value;
  }
}

void handle_command(char *cmd) {
  if (cmd[0] == 0)
    return;
  if (so_strcmp(cmd, "help") == 0) {
    uart_writeText("---------------------------------------------------\n");
    uart_writeText("  StellarOS help:\n");
    uart_writeText("---------------------------------------------------\n");
    uart_writeText("  help        print this help message\n");
    uart_writeText("  version     print version information\n");
    uart_writeText("  exit        exit StellarOS\n");
    uart_writeText("---------------------------------------------------\n");
  } else if (so_strcmp(cmd, "version") == 0) {
    uart_writeText("StellarOS version: ");
    uart_writeText(STELLAROS_VERSION_STRING_FULL);
    uart_writeText("\n");
    uart_writeText("Written by wheatfox(enkerewpo@hotmail.com)\n");
  } else if (so_strcmp(cmd, "exit") == 0) {
    asm volatile("b exit_kernel");
  } else {
    uart_writeText("Unknown command: ");
    uart_writeText(cmd);
    uart_writeText("\n");
  }
}

void shell_loop() {
  while (1) {
    print_prefix();
    int i = 0;
    while (1) {
      char c = uart_readByte();
      if (c == 13) {
        uart_writeText("\n");
        handle_command(buffer);
        so_memset(buffer, 0, 2048);
        break;
      } else if (c == 127) {
        if (i > 0) {
          uart_writeText("\b \b");
          i--;
        }
      } else if (c >= 32) {
        if (i < 2047) {
          buffer[i++] = c;
          uart_writeChar(c);
        }
      }
    }
  }
}