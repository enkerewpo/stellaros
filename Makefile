INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = build
BUILD_ARGS = -I $(INCLUDE_DIR) -g -O0
GCCPATH = /opt/gcc-arm/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin # optional for gcc cross compiler

CFILES = $(wildcard src/*.c)
SFILES = $(wildcard src/asm/*.s)
OFILES = $(CFILES:.c=.o) $(SFILES:.s=.o)
LLVMPATH = /bin
CLANGFLAGS = -Wall -O0 -nostdlib -mcpu=cortex-a72+nosimd -ffreestanding

all: clean stellaros-kernel.img

%.o: %.c
	$(LLVMPATH)/clang --target=aarch64-elf $(CLANGFLAGS) -c $< -o $@ $(BUILD_ARGS)

%.o: %.s
	$(LLVMPATH)/clang --target=aarch64-elf $(CLANGFLAGS) -c $< -o $@ $(BUILD_ARGS)

stellaros-kernel.img: $(OFILES)
	$(LLVMPATH)/ld.lld -m aarch64elf -nostdlib $(OFILES) -T $(SRC_DIR)/asm/link.ld -o $(BUILD_DIR)/stellaros-kernel.elf
	$(LLVMPATH)/llvm-objcopy -O binary $(BUILD_DIR)/stellaros-kernel.elf $(BUILD_DIR)/stellaros-kernel.img

clean:
	/bin/rm $(BUILD_DIR)/stellaros-kernel.elf $(BUILD_DIR)/stellaros-kernel.img 2>/dev/null || true