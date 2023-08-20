#include "interrupts/irq.h"
#include "interrupts/pic.h"
#include "binary.h"
#include "print.h"
#include <stddef.h>

#define PIC_REMAP_OFFSET        0x20

IRQHandler g_IRQHandlers[16];

void IRQ_Handler(Registers* regs) {
    int irq = regs->interrupt - PIC_REMAP_OFFSET;

    if (g_IRQHandlers[irq] == NULL) {
        kprintf("Unhandled IRQ %x...\n", irq);
    } else {
        g_IRQHandlers[irq](regs);
    }

    PIC_SendEndOfInterrupt(irq);
}

void IRQ_Initialize() {
    PIC_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);

    for (int i = 0; i < 16; i++)
        ISR_RegisterHandler(PIC_REMAP_OFFSET + i, IRQ_Handler);

    EnableInterrupts();
}

void IRQ_RegisterHandler(int irq, IRQHandler handler) {
    g_IRQHandlers[irq] = handler;
}
