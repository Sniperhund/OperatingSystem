global header_start

section .text
bits 32
start:
    cli

    extern kernel_main
    call kernel_main

    hlt