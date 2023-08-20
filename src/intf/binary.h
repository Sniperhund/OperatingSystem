#pragma once

#include <stdint.h>

#define FLAG_SET(number, flag) number |= (flag)
#define FLAG_UNSET(number, flag) number &= ~(flag)

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void outw(uint16_t port, uint16_t value);
uint16_t inw(uint16_t port);
void iowait();

// TODO: Move these to their own header
void __attribute__((cdecl)) halt();
void __attribute__((cdecl)) panic();

uint8_t __attribute__((cdecl)) EnableInterrupts();
uint8_t __attribute__((cdecl)) DisableInterrupts();