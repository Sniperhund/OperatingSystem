#include "binary.h"
#include "print.h"

#include <stdint.h>

void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port)
{
   uint8_t ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

void outw(uint16_t port, uint16_t value) {
    asm volatile("out %1, %0" : : "dN" (port), "a" (value));
}

uint16_t inw(uint16_t port)
{
   uint16_t ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

#define UNUSED_PORT 0x80

void iowait() {
    outb(UNUSED_PORT, 0);
}

void __attribute__((cdecl)) panic() {
    kprintf("\nPANIC\n");
    DisableInterrupts();
    halt();
}
