#ifndef _UART_H
#define _UART_H

#if defined(RPI4)
    #define IO_BASE_ADDR    0xfe200000 // Base address for Raspberry Pi 4
#else
    #define IO_BASE_ADDR    0x3f200000 // Base address for Raspberry Pi 3 and earlier models
#endif

#define UART0_DR        IO_BASE_ADDR + 0x1000
#define UART0_FR        IO_BASE_ADDR + 0x1018
#define UART0_CR        IO_BASE_ADDR + 0x1030
#define UART0_LCRH      IO_BASE_ADDR + 0x102c
#define UART0_FBRD      IO_BASE_ADDR + 0x1028
#define UART0_IBRD      IO_BASE_ADDR + 0x1024
#define UART0_IMSC      IO_BASE_ADDR + 0x1038

unsigned char read_char(void);
void write_char(unsigned char c);
void write_string(const char *string);
void init_uart(void);

#endif
