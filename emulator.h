#ifndef EMULATOR_H
#define EMULATOR_H

#include "hardware.h"

uint8_t r[16];
uint16_t addres = 0;
uint16_t word;
char eq = 'e';

static inline void tick(uint16_t tick_rate) {
#ifdef _WIN32
    Sleep(1000 / tick_rate);
#else
    usleep((uint64_t)(1000000 / tick_rate));
#endif
}

void chip() {
    while (addres < sizeof(rom) / sizeof(rom[0])) {
        // подготовка данных
        word = rom[addres];
        displayUART(0);
        joystick(0xFD, 0xFE, 0xFC);
        if (ram[0xFF] != 0) {break;}
        
        tick(200000000); // скорость
        
        // переменные
        uint8_t instr = (word >> 13) & 7;
        uint8_t rn = (word >> 12) & 1;
        uint8_t rsn = (word >> 8) & 0x0F;
        uint8_t n = word & 0xFF;
        
        // эмулятор
        //printf("instruction: %i\n", instr);
        if (instr == 0) {
            if (rn == 0) {r[rsn] = n;}
            else {r[rsn] = r[n];}
        } else if (instr == 1) {
            if (rn == 0) {r[rsn] = r[rsn] + n;}
            else {r[rsn] = r[rsn] + r[n];}
        } else if (instr == 2) {
            if (rn == 0) {r[rsn] = r[rsn] - n;}
            else {r[rsn] = r[rsn] - r[n];}
        } else if (instr == 3) {
            uint8_t time = r[rsn];
            if (rn == 0) {
                r[rsn] = ram[n];
                ram[n] = time;
            } else {
                r[rsn] = ram[r[n]];
                ram[r[n]] = time;
            }
        } else if (instr == 4) {
            if (rn == 0) {
                if (r[rsn] == n) {eq = 'e';}
                if (r[rsn] > n) {eq = 'h';}
                if (r[rsn] < n) {eq = 'l';}
            } else {
                if (r[rsn] == r[n]) {eq = 'e';}
                if (r[rsn] > r[n]) {eq = 'h';}
                if (r[rsn] < r[n]) {eq = 'l';}
            }
        } else if (instr == 5) {
            if (eq == 'e') {
                if (rn == 0) {
                    addres = rsn * 0x100 + n - 1;
                } else {
                    addres = r[rsn] - 1;
                }
            }
        } else if (instr == 6) {
            if (eq == 'h') {
                if (rn == 0) {
                    addres = rsn * 0x100 + n - 1;
                } else {
                    addres = r[rsn] - 1;
                }
            }
        } else if (instr == 7) {
            if (eq == 'l') {
                if (rn == 0) {
                    addres = rsn * 0x100 + n - 1;
                } else {
                    addres = r[rsn] - 1;
                }
            }
        }
        addres++;
    }
    printf("\n===============\n");
    printf("r0: %i | r1: %i | r2: %i | r3: %i\n-----------------------------\nr4: %i | r5: %i | r6: %i | r7: %i\n-----------------------------\nr8: %i | r9: %i | rA: %i | rB: %i\n-----------------------------\nrC: %i | rD: %i | rE: %i | rF: %i\n", r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8], r[9], r[10], r[11], r[12], r[13], r[14], r[15]);
    return;
}

#endif