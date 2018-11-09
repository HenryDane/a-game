enum color_t {
    NORMAL = 7, DARK_RED = 4, BRIGHT_RED = 12, GREEN = 10, LT_BLUE = 11, DEAD = 24, YELLOW = 14, GR_GREEN = 162, R_RED = 196
};

bool set_color(color_t c);
bool jump_xy(int x, int y);
bool jump_xync(int x, int y);
bool clear_screen(void);
bool print_nchr(int n, char c);
char get_key(void);

void do_msg(std::string msg);


bool paint_grid();

