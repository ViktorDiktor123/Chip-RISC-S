mov r0 x1
add r1 r0
mov r2 r1
mov r1 r0
mov r0 r2
mov r3 x0
mov r4 x0A
cmp r2 r4
jlo x00F
sub r2 r4
mov r4 x1
add r3 r4
mov r4 x0A
cmp r0 r0
jeq x007
mov r4 x30
add r3 r4
add r2 r4
ram r3 xFA
ram r3 xFA
ram r2 xFA
ram r2 xFA
mov r4 x20
ram r4 xFA
ram r4 xFA
cmp r0 xE9
jeq x025
cmp r0 r0
jeq x001
mov r2 x1
ram r2 xFF
