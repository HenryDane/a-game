#include <iostream>
#include "main.h"
#include "windows.h"
#include "console.h"

/*
bool set_color(color_t c){
    int f, b = 0;

    switch (c){
    case NORMAL:
        f = 15; b = 0; break;
    case DARK_RED:
        f = 1; b = 0; break;
    case BRIGHT_RED:
        f = 9; b = 0; break;
    case GREEN:
        f = 10; b = 0; break;
    case LT_BLUE:
        f = 14; b = 0; break;
    case DEAD:
        f = 8; b = 4; break;
    case YELLOW:
        f = 11; b = 0; break;
    case GR_GREEN:
        f = 0; b = 10; break;
    case R_RED:
        f = 0; b = 9; break;
    default:
        f = 9; b = 9; break;
    }

    std::cout << "\033[38;5;" << f << "m";
    std::cout << "\033[48;5;" << b << "m";
    return true;
}
*/

bool jump_xy(int x, int y) {
/*    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > S_WIDTH) x = S_WIDTH;
    if (y > S_HEIGHT + 4) y = S_HEIGHT + 4;
    jump_xync(x, y);

    return true;
*/
}

/*
bool jump_xync(int x, int y) {
    // i dont know why its (y + 1) but dont change it please
    std::cout << "\033[" << y + 1 << ";" << x << "H";
    return true;
}
*/

/*
bool paint_grid() {
    for (int y = 0; y <= S_HEIGHT; y++){
        jump_xy(0, y + 2);
        for (int x = 0; x <= S_WIDTH; x++){
            std::cout << ".";
        }
    }
    return true;
}
*/

/*
bool clear_screen() {
    std::cout << "\033[2J";
    return true;
}
*/

/*
bool print_nchr(int n, char c){
    for (int i = 0; i < n; i++)
        std::cout << c;
    return true;
}
*/

/*
bool print_nchrv(int n, int x, char c){
    for (int i = 0; i < n; i++){
        jump_xy(x, i);
        std::cout << c;
    }
    return true;
}
*/

// windows only
/*
char get_key(void) {
    DWORD mode, cc;
    HANDLE h = GetStdHandle( STD_INPUT_HANDLE );

    if (h == NULL) {
        return 0; // console not found
    }

    GetConsoleMode( h, &mode );
    SetConsoleMode( h, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT) );
    TCHAR c = 0;
    ReadConsole( h, &c, 1, &cc, NULL );
    SetConsoleMode( h, mode );
    return c;
}
*/

/*
bool init_console_win(void){
#ifdef _WIN32
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        //return GetLastError();
        return false;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        //return GetLastError();
        return false;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        //return GetLastError();
        return false;
    }
#elif
    return true;
#endif
}
*/
