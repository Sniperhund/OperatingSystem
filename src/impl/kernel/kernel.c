#include "print.h"
#include "gdt.h"
#include "drivers/terminal.h"

void kernel_main() {
    GDTInitialize();

    TerminalClear();
    
    kprintf("You are very handsome!");
}