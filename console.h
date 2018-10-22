enum color_t {
    NORMAL = 7, DARK_RED = 4, BRIGHT_RED = 12, GREEN = 10, LT_BLUE = 11
};

bool set_color(color_t c);
bool jump_xy(unsigned int x, unsigned int y);
bool clear_screen(void);
bool print_nchr(int n, char c);
char get_key(void);
