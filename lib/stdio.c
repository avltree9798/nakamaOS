#include "stdint.h"
#include "stdarg.h"
#include "../include/uart.h"

#define BUFFER_SIZE 1024

static int read_string(char *buffer, int position, const char *string, int max)
{
    int index = 0;
    while (string[index] != '\0' && (position + index) < (max - 1)) {
        buffer[position + index] = string[index];
        index++;
    }
    buffer[position + index] = '\0';
    return index;
}


static int hex_to_string(char *buffer, int position, uint64_t digits, int max) {
    char digits_buffer[25];  // Large enough for a 64-bit number in hex
    char digits_map[16] = "0123456789ABCDEF";
    int size = 0;

    do {
        if (size < (sizeof(digits_buffer) - 1)) {
            digits_buffer[size++] = digits_map[digits % 16];
        }
        digits /= 16;
    } while (digits != 0 && (position + size + 1) < max);

    for (int i = size - 1; i >= 0; i--) {
        if ((position + 1) < max) {
            buffer[position++] = digits_buffer[i];
        }
    }

    if (position < max) {
        buffer[position++] = 'H';
    }

    return size + 1;
}

static int udecimal_to_string(char *buffer, int position, uint64_t digits, int max) {
    char digits_buffer[25];
    char digits_map[10] = "0123456789";
    int size = 0;
    do {
        if (size < (sizeof(digits_buffer) - 1)) {
            digits_buffer[size++] = digits_map[digits % 10];
        }
        digits /= 10;
    } while (digits != 0 && (position + size) < max);
    for (int i = size - 1; i >= 0; i--) {
        if (position < max) {
            buffer[position++] = digits_buffer[i];
        }
    }
    return size;
}

static int decimal_to_string(char *buffer, int position, int64_t digits, int max)
{
    int size = 0;
    if (digits < 0) {
        if (position < max - 1) {
            buffer[position++] = '-';
            size = 1;
        }
        digits = -digits;
    }

    size += udecimal_to_string(buffer, position, (uint64_t)digits, max - size);
    return size;
}

static void write_console(const char *buffer, int size)
{
    for (int i = 0; i < size; i++) {
        write_char(buffer[i]);
    }
}

int printk(const char *format, ...)
{
    char buffer[BUFFER_SIZE];
    int buffer_size = 0;
    int64_t integer = 0;
    char *string = 0;
    va_list args;

    va_start(args, format);

    for (int i = 0; format[i] != '\0' && buffer_size < BUFFER_SIZE; i++) {
        if (format[i] != '%') {
            if (buffer_size < BUFFER_SIZE - 1) {
                buffer[buffer_size++] = format[i];
            }
        } else {
            i++;
            switch (format[i]) {
                case 'x':
                    integer = va_arg(args, int64_t);
                    buffer_size += hex_to_string(buffer, buffer_size, (uint64_t)integer, BUFFER_SIZE - buffer_size);
                    break;

                case 'u':
                    integer = va_arg(args, int64_t);
                    buffer_size += udecimal_to_string(buffer, buffer_size, (uint64_t)integer, BUFFER_SIZE - buffer_size);
                    break;

                case 'd':
                    integer = va_arg(args, int64_t);
                    buffer_size += decimal_to_string(buffer, buffer_size, integer, BUFFER_SIZE - buffer_size);
                    break;

                case 's':
                    string = va_arg(args, char*);
                    buffer_size += read_string(buffer, buffer_size, string, BUFFER_SIZE - buffer_size);
                    break;

                default:
                    if (buffer_size < BUFFER_SIZE - 1) {
                        buffer[buffer_size++] = '%';
                        i--;
                    }
                    break;
            }
        }
    }

    write_console(buffer, buffer_size < BUFFER_SIZE ? buffer_size : BUFFER_SIZE - 1);
    va_end(args);

    return buffer_size;
}