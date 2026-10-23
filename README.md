# Chip-RISC-S
Chip RISC-S (Super RISC)

# using
in the folder with the chip.c There should be a code.s file with assembly code
compile chip.c (gcc chip.c -o chip) and run (./chip)

# code
ram - swap register and RAM cell
mov - move num/register value to register
add - adder
sub - subtraction
cmp - examination
jeq - jump if equal
jlo - jump if low
jhi - jump if high
=========================================
registers r0-rF, x00-xFF
numbers - hex, xNNNNNN...
ram x00-xFF
code x000-xFFF
