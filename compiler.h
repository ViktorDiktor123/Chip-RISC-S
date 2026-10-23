#ifndef COMPILER_H
#define COMPILER_H

uint16_t parse_hex_16(char str[]) {
    if (str[0] == 'x' || str[0] == 'X') {
        return (uint16_t)strtol(&str[1], NULL, 16);
    }
    return (uint16_t)strtol(str, NULL, 16);
}

uint8_t parse_reg(const char *str) {
    if (str[0] == 'r' || str[0] == 'R') {
        return str[1] - '0';
    }
    return 0;
}

void compiler(char code[], uint32_t len) {
    uint16_t idROM = 0;
    char cmd[3][8] = {"", "", ""}; 
    char word[8] = "";             
    uint8_t chto = 0;              

    for (uint32_t id = 0; id < len; id++) {
        char ch = code[id];
        
        if (ch == '\0') {
            break;
        } 
        if (ch == '\r') {
            continue; 
        }
        
        if (ch != ' ' && ch != '\n') {
            if (strlen(word) < 7) { 
                char sym[2] = {ch, '\0'};
                strcat(word, sym);
            }
        }
        
        if (ch == ' ' || (id == len - 1 && ch != '\n')) {
            if (strlen(word) > 0) { 
                if (chto < 3) {
                    strncpy(cmd[chto], word, 7);
                    chto++;
                }
                word[0] = '\0'; 
            }
        }
        
        if (ch == '\n' || id == len - 1) {
            if (chto < 3 && strlen(word) > 0) {
                strncpy(cmd[chto], word, 7);
            }
            
            if (strlen(cmd[0]) == 0) {
                chto = 0;
                word[0] = '\0';
                continue;
            }

            uint8_t instr = 0;
            uint8_t rn = 0;
            uint8_t rsn = 0;
            uint8_t n = 0;
            
            if (!strcmp(cmd[0], "mov"))      { instr = 0; } 
            else if (!strcmp(cmd[0], "add")) { instr = 1; } 
            else if (!strcmp(cmd[0], "sub")) { instr = 2; } 
            else if (!strcmp(cmd[0], "ram")) { instr = 3; } 
            else if (!strcmp(cmd[0], "cmp")) { instr = 4; } 
            else if (!strcmp(cmd[0], "jeq")) { instr = 5; } 
            else if (!strcmp(cmd[0], "jhi")) { instr = 6; } 
            else if (!strcmp(cmd[0], "jlo")) { instr = 7; }
            else {
                printf("Ошибка компиляции: неизвестная команда '%s' на шаге %d\n", cmd[0], idROM);
            }
            
            if (instr < 5) {
                rsn = parse_reg(cmd[1]); 

                if (cmd[2][0] == 'r' || cmd[2][0] == 'R') {
                    rn = 1; 
                    n = parse_reg(cmd[2]);
                } else {
                    rn = 0; 
                    n = (uint8_t)parse_hex_16(cmd[2]);
                }
            } else {
                if (cmd[1][0] == 'r' || cmd[1][0] == 'R') {
                    rn = 1; 
                    rsn = parse_reg(cmd[1]);
                    n = 0;
                } else {
                    rn = 0; 
                    
                    uint16_t target_addr = parse_hex_16(cmd[1]); 

                    rsn = (target_addr >> 8) & 0x0F; 
                    n = target_addr & 0xFF;          
                }
            }
            
            rom[idROM] = (instr << 13) | (rn << 12) | (rsn << 8) | n;
            idROM++;
            
            chto = 0;
            word[0] = '\0';
            strcpy(cmd[0], "");
            strcpy(cmd[1], "");
            strcpy(cmd[2], "");
        }
    }
    return;
}

#endif