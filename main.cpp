#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include "main.h"
#include "console.h"
#include "game.h"
#include "interact.h"
#include "patch.h"
#include "tex_print.h"

// TODO add selective redraw
//      add tutorial level
//      make game victory on final door exit not timer fix

// globals lol
int cha_x = S_WIDTH / 2;
int cha_y = S_HEIGHT / 2;
int score = 5;
int shield = -1002; // signals first turn
std::vector<entity_t> entities;
std::vector<enemy_t> enemies;
int global_uuid_next = 0;

bool regen_on = true;
int timer_on = -1;

int global_score = 0;
int level = 0;

int main(){
    // configure random
    srand( time( NULL) );

    // print start screen
    jump_xy(0, 0); std::cout << "Version 0.1.0000";
    set_color(color_t::GREEN);
    jump_xy(39, 9); std::cout << "                                          " << ((char) 10);
    jump_xy(39, 10); std::cout << "   __ _        __ _  __ _ _ __ ___   ___  " << ((char) 10);
    jump_xy(39, 11); std::cout << "  / _` |_____ / _\\`|/ _` | '_ ` _ \\ / _ \\ " << ((char) 10);
    jump_xy(39, 12); std::cout << " | (_| |_____| (_| | (_| | | | | | |  __/ " << ((char) 10);
    jump_xy(39, 13); std::cout << "  \\__,_|      \\__, |\\__,_|_| |_| |_|\\___| " << ((char) 10);
    jump_xy(39, 14); std::cout << "              |___/                       " << ((char) 10);
    jump_xy(39, 15); std::cout << "                                          " << ((char) 10);
    set_color(color_t::NORMAL);
    jump_xy(39, 16); std::cout << "              l e v e l   "; set_color(color_t::BRIGHT_RED); std::cout << "# " << level << "             " << ((char) 10);
    set_color(color_t::NORMAL);
    jump_xy(39, 18); std::cout << "       Press any key to begin a game      " << std::endl;

    // allocate memory in vectors to avoid issues
    init_registry();
    entities.reserve(100);
    enemies.reserve(30);

    register_object(global_uuid_next++, 2, 0, 0); // register player

    generate_terrain();

    player_set_safe(); // go to safe location

    get_key();
    clear_screen();
    draw_level_screen(0);

    while (true) {
        char c = get_key(); // get input

        if (shield > 0) shield--; // decrease shield
        if (timer_on > 0) timer_on --;
        if (timer_on == 0 && level == 9){
            do_win_screen();
        } else if (timer_on == 0 && level != 9) {
            score -= 1000;
        }

        // control logic
        switch (c){
        case 'w':
            update_object(0, 0, -1);
            break;
        case 'd':
            update_object(0, 1, 0);
            break;
        case 's':
            update_object(0, 0, 1);
            break;
        case 'a':
            update_object(0, -1, 0);
            break;
        case 'q':
            if (score > 10 &&
                level != 6 &&
                level != 7 &&
                level != 8) { // activates ten turn shield for a score cost of 5
                shield = 10;
                score -= 5;
            }
            break;
        case 'e': // uses a grenade with a cost of 5
            if (score > 10 &&
                level != 6 &&
                level != 7 &&
                level != 8) {
                score -= 5;
                draw_explosion(cha_x, cha_y, 2);
                get_key();

                for (unsigned int i = 0; i < enemies.size(); i++){
                    if (abs(enemies[i]._x - cha_x) < 3 &&
                        abs(enemies[i]._y - cha_y) < 3){
                        enemies[i]._score -= -10;
                        score += 5;
                    }
                }
            }
            break;
        case 'l': score += 10; break;
        case ';': generate_terrain(); break;
        case 'n': damage_object_x(cha_x, 0); break;
        case 'm': damage_object_y(cha_y, 0); break;
        default:
            update_object(0, 0, 0);
            break;
        }

        // draw screen
        draw();

        for (unsigned int i = 0; i < enemies.size(); i++){
            tick_enemy(enemies[i], entities, cha_x, cha_y);
        }

        // first 3 turns are immune to score changes
        if (shield <= -1000){
            shield = 0;
            score = 5;
        }

        // game over check
        if (score < -20) {
            do_death_screen();
            return 0;
        }
    }
    return 0;
}

void draw(void){
    // draw logic
    clear_screen();

    // character
    set_color(color_t::GREEN);
    jump_xy(cha_x, cha_y + 2);
    if (shield <= 0) std::cout << "0";
    if (shield > 0) std::cout << "@";
    set_color(color_t::NORMAL);

    // entities
    for (entity_t e : entities){
        if (e.t < 0) continue;
        jump_xy(e.x, e.y + 2);
        switch (e.t){
        case 1:
        case 0:
            std::cout << "+"; break;
        case 2:
            set_color(color_t::DARK_RED);
            std::cout << "!";
            set_color(color_t::NORMAL);
            break;
        case 3:
            set_color(color_t::YELLOW);
            std::cout << "$";
            set_color(color_t::NORMAL);
            break;
        case 4:
            std::cout << "#"; break;
        case 5:
            set_color(color_t::GR_GREEN);
            std::cout << "D";
            set_color(color_t::NORMAL);
            break;
        case 6:
            std::cout << "T"; break;
        default:
            break;
        }
    }

    // enemies
    for (enemy_t e : enemies){
        jump_xy(e._x, e._y + 2);
        if (e._state >= 0) set_color(color_t::LT_BLUE);
        else if (e._state < 0) set_color(color_t::DEAD);

        if (e._t == 2 || e._t == 3) {
            if (e._state < 0){
                std::cout << "-";
            } else if (e._state >= 10){
                std::cout << "*";
            } else {
                std::cout << e._state;
            }
        } else if (e._t == 4) {
            if (e._state >= 0) set_color(color_t::R_RED);
            std::cout << "O";
        } else {
            std::cout << "O";
        }

        set_color(color_t::NORMAL);
    }

    // score
    jump_xy(0, 0);
    std::cout << "SCORE:" << score << " SHIELD:" << ((shield <= 0) ? 0 : shield);
    if (timer_on < 0) {
        std::cout << " TURNS: infinity";
    } else {
        std::cout << " TURNS:" << timer_on;
    }

    // debug
    jump_xy(10, 0);

    // reset cursor
    jump_xy(0, 0);

    // border
    jump_xy(0, 1);
    print_nchr(S_WIDTH + 1, '=');
    jump_xync(0, 3 + S_HEIGHT);
    print_nchr(S_WIDTH + 1, '=');
}

void draw_explosion(int x, int y, int r){
    set_color(color_t::BRIGHT_RED);
    for (int k = -r; k <= r; k++){
        for (int m = -r; m <= r; m++){
            jump_xy(x + k, y + m + 2);
            std::cout << "*";
        }
    }
    set_color(color_t::NORMAL);
}

void draw_level_screen(int lvl){
    print_lvl_lbl(30,11);
    set_color(color_t::GREEN);
    switch(lvl){
        case 0: print_00(68,11); break;
        case 1: print_01(68,11); break;
        case 2: print_02(68,11); break;
        case 3: print_03(68,11); break;
        case 4: print_04(68,11); break;
        case 5: print_05(68,11); break;
        case 6: print_06(68,11); break;
        case 7: print_07(68,11); break;
        case 8: print_08(68,11); break;
        case 9: print_09(68,11); break;
        case 10: print_10(68,11); break;
    }
    set_color(color_t::NORMAL);
    jump_xy(47, 19); std::cout << "Press any key to begin";
    get_key();
}

void do_win_screen() {
    char NL = 10;

    int x = 30;
    int y = 11;

    get_key();
    draw();
    get_key();
    clear_screen();

    // print info
    set_color(color_t::GREEN);
        jump_xy(x, y); std::cout << " __   __                   __        __  _           _ " << NL;
    jump_xy(x, y + 1); std::cout << " \\ \\ / /   ___    _   _    \\ \\      / / (_)  _ __   | |" << NL;
    jump_xy(x, y + 2); std::cout << "  \\ V /   / _ \\  | | | |    \\ \\ /\\ / /  | | | '_ \\  | |" << NL;
    jump_xy(x, y + 3); std::cout << "   | |   | (_) | | |_| |     \\ V  V /   | | | | | | |_|" << NL;
    jump_xy(x, y + 4); std::cout << "   |_|    \\___/   \\__,_|      \\_/\\_/    |_| |_| |_| (_)" << NL;
    set_color(color_t::NORMAL);
    jump_xy(x, y + 6); std::cout << "                   Score: " << score << NL;

    get_key();
    get_key();

    exit(0);

}

void do_death_screen() {
    char NL = 10;

    int x = 30;
    int y = 11;

    get_key();
    draw();
    get_key();
    clear_screen();

    // print info
    set_color(color_t::BRIGHT_RED);
        jump_xy(x, y); std::cout << "   ____                         ___" << NL;
    jump_xy(x, y + 1); std::cout << "  / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __" << NL;
    jump_xy(x, y + 2); std::cout << " | |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__|" << NL;
    jump_xy(x, y + 3); std::cout << " | |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |" << NL;
    jump_xy(x, y + 4); std::cout << "  \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|" << NL;
    set_color(color_t::NORMAL);
    jump_xy(x, y + 6); std::cout << "                   Score: " << score << NL;

    get_key();
    get_key();

    exit(0);

}
