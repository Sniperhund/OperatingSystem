global header_start
global start

section .text
bits 32
start:
    cli

    extern kernel_main
    call kernel_main

    hlt

global halt
halt:
    hlt

global EnableInterrupts
EnableInterrupts:
    sti
    ret

global DisableInterrupts
DisableInterrupts:
    cli
    ret

section .bss
align 4096
stack_top:
    resb 4096
stack_bottom: