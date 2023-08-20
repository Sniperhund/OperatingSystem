#include "print.h"
#include "interrupts/gdt.h"
#include "interrupts/idt.h"
#include "interrupts/irq.h"
#include "interrupts/isr.h"
#include "drivers/terminal.h"

void timer(Registers* reg);

void kernel_main() {
    TerminalClear();

    GDTInitialize();
    IDT_Initialize();
    ISR_Initialize();
    IRQ_Initialize();
    
    IRQ_RegisterHandler(0, timer);

    while (1) {
        asm volatile ("nop");
    }
}

void timer(Registers* reg) {
    kprintf("Time is funnnn!");
}