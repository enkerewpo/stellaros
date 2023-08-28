#include "shell.h"
#include "io_virt.h"
#include "version.h"

char buffer[2048]; // for shell input

void print_prefix() { print("StellarOS> "); }

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
    print("---------------------------------------------------\n");
    print("  StellarOS help:\n");
    print("---------------------------------------------------\n");
    print("  help        print this help message\n");
    print("  version     print version information\n");
    print("  exit        exit StellarOS\n");
    print("---------------------------------------------------\n");
  } else if (so_strcmp(cmd, "version") == 0) {
    print("StellarOS version: ");
    print(STELLAROS_VERSION_STRING_FULL);
    print("\n");
    print("Written by wheatfox(enkerewpo@hotmail.com)\n");
  } else if (so_strcmp(cmd, "exit") == 0) {
    asm volatile("b exit_kernel");
  } else {
    print("Unknown command: ");
    print(cmd);
    print("\n");
  }
}

void shell_loop() {
  while (1) {
    print_prefix();
    int i = 0;
    while (1) {
      char c = getchar();
      if (c == 13) {
        print("\n");
        handle_command(buffer);
        so_memset(buffer, 0, 2048);
        break;
      } else if (c == 127) {
        if (i > 0) {
          print("\b \b");
          i--;
        }
      } else if (c >= 32) {
        if (i < 2047) {
          buffer[i++] = c;
          putchar(c);
        }
      }
    }
  }
}