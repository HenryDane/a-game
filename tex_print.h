char NL = 10;

// 36 chars wide
void print_lvl_lbl(int x, int y){
        jump_xy(x, y); std::cout << "  _                             _   " << NL;
    jump_xy(x, y + 1); std::cout << " | |       ___  __   __   ___  | |  " << NL;
    jump_xy(x, y + 2); std::cout << " | |      / _ \\ \\ \\ / /  / _ \\ | |  " << NL;
    jump_xy(x, y + 3); std::cout << " | |___  |  __/  \\ V /  |  __/ | |  " << NL;
    jump_xy(x, y + 4); std::cout << " |_____|  \\___|   \\_/    \\___| |_|  " << NL;
}

// 17 chars wide
void print_00(int x, int y){
        jump_xy(x, y); std::cout << "   ___     ___   " << NL;
    jump_xy(x, y + 1); std::cout << "  / _ \\   / _ \\  " << NL;
    jump_xy(x, y + 2); std::cout << " | | | | | | | | " << NL;
    jump_xy(x, y + 3); std::cout << " | |_| | | |_| | " << NL;
    jump_xy(x, y + 4); std::cout << "  \\___/   \\___/  " << NL;
}

// 12 chars wide
void print_01(int x, int y){
        jump_xy(x, y); std::cout << "   ___    _ " << NL;
    jump_xy(x, y + 1); std::cout << "  / _ \\  / | " << NL;
    jump_xy(x, y + 2); std::cout << " | | | | | | " << NL;
    jump_xy(x, y + 3); std::cout << " | |_| | | | " << NL;
    jump_xy(x, y + 4); std::cout << "  \\___/  |_| " << NL;
}

void print_02(int x, int y){
        jump_xy(x, y); std::cout << "   ___    ____" << NL;
    jump_xy(x, y + 1); std::cout << "  / _ \\  |___ \\" << NL;
    jump_xy(x, y + 2); std::cout << " | | | |   __) |" << NL;
    jump_xy(x, y + 3); std::cout << " | |_| |  / __/" << NL;
    jump_xy(x, y + 4); std::cout << "  \\___/  |_____|" << NL;
}

void print_03(int x, int y){
        jump_xy(x, y); std::cout << "   ___    _____" << NL;
    jump_xy(x, y + 1); std::cout << "  / _ \\  |___ /" << NL;
    jump_xy(x, y + 2); std::cout << " | | | |   |_ \\" << NL;
    jump_xy(x, y + 3); std::cout << " | |_| |  ___) |" << NL;
    jump_xy(x, y + 4); std::cout << "  \\___/  |____/" << NL;
}

void print_04(int x, int y){
        jump_xy(x, y); std::cout << "   ___    _  _" << NL;
    jump_xy(x, y + 1); std::cout << "  / _ \\  | || |" << NL;
    jump_xy(x, y + 2); std::cout << " | | | | | || |_" << NL;
    jump_xy(x, y + 3); std::cout << " | |_| | |__   _|" << NL;
    jump_xy(x, y + 4); std::cout << "  \\___/     |_|" << NL;
}

void print_05(int x, int y){
        jump_xy(x, y); std::cout << "   ___    ____" << NL;
    jump_xy(x, y + 1); std::cout << "  / _ \\  | ___|" << NL;
    jump_xy(x, y + 2); std::cout << " | | | | |___ \\" << NL;
    jump_xy(x, y + 3); std::cout << " | |_| |  ___) |" << NL;
    jump_xy(x, y + 4); std::cout << "  \\___/  |____/" << NL;
}

void print_06(int x, int y){
        jump_xy(x, y); std::cout << "   ___     __" << NL;
    jump_xy(x, y + 1); std::cout << "  / _ \\   / /_" << NL;
    jump_xy(x, y + 2); std::cout << " | | | | | '_ \\" << NL;
    jump_xy(x, y + 3); std::cout << " | |_| | | (_) |" << NL;
    jump_xy(x, y + 4); std::cout << "  \\___/   \\___/" << NL;
}

void print_07(int x, int y){
        jump_xy(x, y); std::cout << "   ___    _____" << NL;
    jump_xy(x, y + 1); std::cout << "  / _ \\  |___  |" << NL;
    jump_xy(x, y + 2); std::cout << " | | | |    / /" << NL;
    jump_xy(x, y + 3); std::cout << " | |_| |   / /" << NL;
    jump_xy(x, y + 4); std::cout << "  \\___/   /_/" << NL;
}

void print_08(int x, int y){
        jump_xy(x, y); std::cout << "   ___     ___" << NL;
    jump_xy(x, y + 1); std::cout << "  / _ \\   ( _ )" << NL;
    jump_xy(x, y + 2); std::cout << " | | | |  / _ \\" << NL;
    jump_xy(x, y + 3); std::cout << " | |_| | | (_) |" << NL;
    jump_xy(x, y + 4); std::cout << "  \\___/   \\___/" << NL;
}

void print_09(int x, int y){
        jump_xy(x, y); std::cout << "   ___     ___" << NL;
    jump_xy(x, y + 1); std::cout << "  / _ \\   / _ \\" << NL;
    jump_xy(x, y + 2); std::cout << " | | | | | (_) |" << NL;
    jump_xy(x, y + 3); std::cout << " | |_| |  \\__, |" << NL;
    jump_xy(x, y + 4); std::cout << "  \\___/     /_/" << NL;
}

void print_10(int x, int y){
        jump_xy(x, y); std::cout << "  _    ___" << NL;
    jump_xy(x, y + 1); std::cout << " / |  / _ \\" << NL;
    jump_xy(x, y + 2); std::cout << " | | | | | |" << NL;
    jump_xy(x, y + 3); std::cout << " | | | |_| |" << NL;
    jump_xy(x, y + 4); std::cout << " |_|  \\___/" << NL;
}
