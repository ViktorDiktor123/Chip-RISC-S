#ifndef HARDWARE_H
#define HARDWARE_H

uint8_t UARTS[3][2] = {
    {0xFA, 0xFB},
    {0xF6, 0xF7},
    {0xF8, 0xF9}
};

uint8_t I2C[2] = {0xF0, 0xF1};

uint8_t SPIin = 0xF2;
uint8_t SPIout = 0xF3;
uint8_t SPIclock = 0xF4;
uint8_t SPIcs = 0xF5;

#ifndef _WIN32
static struct termios _old_tty;
static bool _tty_initialized = false;
static inline void _restore_tty(void) {
    if (_tty_initialized) {
        tcsetattr(STDIN_FILENO, TCSANOW, &_old_tty);
    }
}
#endif

static inline char key(void) {
#ifdef _WIN32
    if (_kbhit()) {
        return (char)_getch();
    }
    return 0;
#else
    if (!_tty_initialized) {
        struct termios new_tty;
        tcgetattr(STDIN_FILENO, &_old_tty);
        new_tty = _old_tty;
        new_tty.c_lflag &= ~(ICANON | ECHO);
        new_tty.c_cc[VMIN] = 0;
        new_tty.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tty);
        _tty_initialized = true;
        atexit(_restore_tty);
    }
    char ch = 0;
    if (read(STDIN_FILENO, &ch, 1) <= 0) {
        return 0; 
    }
    return ch;
#endif
}

static bool _mouse_initialized = false;
#ifndef _WIN32
static inline void _restore_mouse(void) {
    if (_mouse_initialized) {
        printf("\e[?1003l\e[?1000l");
        fflush(stdout);
    }
}
#endif

static inline bool mouse(uint8_t m[3]) {
#ifdef _WIN32
    (void)m;
    return false;
#else
    if (!_mouse_initialized) {
        printf("\e[?1000h\e[?1003h");
        fflush(stdout);
        _mouse_initialized = true;
        atexit(_restore_mouse);
        
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    }
    char buf[6];
    ssize_t n = read(STDIN_FILENO, buf, 6);
    if (n >= 6) {
        if (buf[0] == 27 && buf[1] == '[' && buf[2] == 'M') {
            m[0] = (uint8_t)(buf[3] - 32);
            m[1] = (uint8_t)(buf[4] - 32);
            m[2] = (uint8_t)(buf[5] - 32);
            return true;
        }
    }
    return false;
#endif
}

void joystick(uint8_t x, uint8_t y, uint8_t btn) {
    uint8_t a[3] = {0, 0, 0};
    mouse(a);
    if (a[0] == 0) {ram[btn] = 255;} 
    ram[x] = a[1];
    ram[y] = a[2];
    return;
}

void displayUART(uint8_t UART) {
    printf("%c", ram[UARTS[UART][0]]);
    ram[UARTS[UART][1]] = key();
    return;
}

#endif