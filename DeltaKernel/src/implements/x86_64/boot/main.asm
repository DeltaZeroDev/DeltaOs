global start
section .text
bits 32 ; in 32b as elf64 breaks when trying to compile
    align   4
    dd      0x1BADB002 ; multiboot1 header, multiboot 2 breaks compilation (idk why)
    dd      0x00
    dd      - (0x1BADB002+0x00)
start:
    mov dword [0xb8000], 0x2f4b2f4f ; "OK"
    hlt
