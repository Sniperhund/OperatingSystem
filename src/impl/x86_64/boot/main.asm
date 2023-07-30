global header_start

section .text
bits 32
start:
    mov dword [0xb8000], 0x2f4f2f4f
    hlt