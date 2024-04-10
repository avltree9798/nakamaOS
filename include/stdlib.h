#ifndef _STDLIB_H
#define _STDLIB_H

#include "stdint.h"

void delay(uint64_t value);
void out_word(uint64_t addr, uint32_t value);
uint32_t in_word(uint64_t addr);

#endif
