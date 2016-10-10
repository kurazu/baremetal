.section .init
.globl _start
_start:
    mov sp,#0x8000
    bl play
hang:
    b hang
