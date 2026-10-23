#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

uint16_t rom[4096];
uint32_t rom_index = 0;
uint8_t ram[256];

#include "compiler.h"
#include "emulator.h"

int main() {
    FILE *file = fopen("code.s", "r");
    if (file == NULL) {
        printf("Ошибка: Не удалось открыть файл с кодом!\n");
        return 1;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *code = (char *)malloc(file_size + 1);
    if (code == NULL) {
        printf("Ошибка: Не удалось выделить память под код!\n");
        fclose(file);
        return 1;
    }
    
    size_t bytes_read = fread(code, 1, file_size, file);
    code[bytes_read] = '\0';
    fclose(file);
    
    printf("Компиляция файла... (размер: %ld байт)\n", file_size);
    compiler(code, bytes_read + 1);
    
    printf("Запуск эмулятора...\n");
    printf("===============\n");
    chip();
    
    free(code);
    return 0;
}