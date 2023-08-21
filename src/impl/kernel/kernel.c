#include "include.h"

void timer(Registers* reg);

void memory_manager_test() {
    int* ptr = (int*) kmalloc(sizeof(int)); // allocate memory for an integer
    if (ptr == NULL) {
        kprint("Memory allocation failed!\n");
        return;
    }
        
    *ptr = 42; // set the value of the integer to 42
        
    kfree(ptr);

    kprint("Memory allocation test passed!\n");
}

void kernel_main() {
    TerminalClear();

    GDTInitialize();
    IDT_Initialize();
    ISR_Initialize();
    IRQ_Initialize();
    
    IRQ_RegisterHandler(0, timer);

    MemoryManagerInitialize();

    memory_manager_test();

    while (1) {
        asm volatile ("nop");
    }
}

void timer(Registers* reg) {
    return;
}