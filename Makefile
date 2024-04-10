GCC=aarch64-elf-gcc
LD=aarch64-elf-ld
OBJCOPY=aarch64-elf-objcopy

CFLAGS=-std=c99 -ffreestanding -mgeneral-regs-only
LDFLAGS=-nostdlib

.PHONY: build clean all test_build test

clean:
	@echo "Cleaning..."
	@rm -f out/*
	@rm -f build/*

# Regular build target
build: TARGET = RPI3
build:
	@echo "Building for $(TARGET)..."
	$(GCC) -D$(TARGET) $(CFLAGS) -o out/boot.o -c boot/boot.S
	$(GCC) -D$(TARGET) $(CFLAGS) -o out/stdlib.o -c lib/stdlib.S
	$(GCC) -D$(TARGET) $(CFLAGS) -o out/main.o -c main.c
	$(GCC) -D$(TARGET) $(CFLAGS) -o out/uart.o -c lib/uart.c
	$(LD) $(LDFLAGS) -T link.lds -o out/kernel out/boot.o out/main.o out/stdlib.o out/uart.o
	$(OBJCOPY) -O binary out/kernel build/kernel.bin

# Test build target
test_build: TARGET = RPI3
test_build:
	@echo "Building for $(TARGET) with test cases..."
	$(GCC) -DRUN_TESTS -D$(TARGET) $(CFLAGS) -o out/boot.o -c boot.S
	$(GCC) -DRUN_TESTS -D$(TARGET) $(CFLAGS) -o out/stdlib.o -c lib/stdlib.S
	$(GCC) -DRUN_TESTS -D$(TARGET) $(CFLAGS) -o out/main.o -c main.c
	$(GCC) -DRUN_TESTS -D$(TARGET) $(CFLAGS) -o out/uart.o -c lib/uart.c
	$(LD) $(LDFLAGS) -T link.lds -o out/kernel out/boot.o out/main.o out/stdlib.o out/uart.o 
	$(OBJCOPY) -O binary out/kernel build/kernel8_test.img

run:
	@echo "Running on QEMU..."
	@qemu-system-aarch64 -M raspi3b -serial stdio -kernel build/kernel.bin

test: test_build
	@echo "Executing UART loopback test with QEMU..."
	@qemu-system-aarch64 -M raspi3b -nographic -serial mon:stdio -kernel build/kernel8_test.img

all: TARGET = RPI3
all:
	@make clean
	@make build TARGET=$(TARGET)
	@make run
