#include <iostream>
#include "main.h"
#include "windows.h"
#include "console.h"

bool set_color(color_t c){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, c);
    return true;
}

bool jump_xy(int x, int y) {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > S_WIDTH) x = S_WIDTH;
    if (y > S_HEIGHT + 4) y = S_HEIGHT + 4;
    COORD p = { x, y };
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), p );
    return true;
}

bool jump_xync(int x, int y) {
    COORD p = { x, y };
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), p );
    return true;
}

bool clear_screen() {
    char fill = ' ';
    COORD tl = {0,0};
    CONSOLE_SCREEN_BUFFER_INFO s;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(console, fill, cells, tl, &written);
    FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(console, tl);
    return true;
}

bool print_nchr(int n, char c){
    for (int i = 0; i < n; i++)
        std::cout << c;
    return true;
}

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

