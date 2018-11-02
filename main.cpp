#include <SFML/Graphics.hpp>
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

// globals lol
int cha_x = S_WIDTH / 2;
int cha_y = S_HEIGHT / 2;
int score = 5;
int global_score = 0;
int level = 0;
int shield = -1002; // signals first turn
std::vector<entity_t> entities;
std::vector<enemy_t> enemies;
int global_uuid_next = 0;

// flags
bool regen_on = true; // do entities regen?
int timer_on = -1; // if >= 0 timer is on
bool dots_on = false; // flag for dots display
bool entity_spawn_lock = false; // flag for deleting everything nearby
bool entity_overlap_check_on = false; // prevent overlap spawn

//gfx shenanigans
sf::RenderTexture renderTexture;

int main(){
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

    while ()
    {



    }

    // configure random
    srand( time( NULL) );

    // print start screen
    draw_title();

    // allocate memory in vectors to avoid issues
    init_registry();
    entities.reserve(100);
    enemies.reserve(30);

    register_object(global_uuid_next++, 2, 0, 0); // register player

    generate_tutorial();

    player_set_safe(); // go to safe location

    get_key();
    clear_screen();
    draw_level_screen(0);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        char c = get_key(); // get input

        if (shield > 0) shield--; // decrease shield
        if (timer_on > 0) timer_on --;
        if (timer_on == 0 && level == 10){
            //do_win_screen();
            make_entity_at(S_WIDTH / 2, S_HEIGHT / 2, 100);
        } else if (timer_on == 0 && level != 10) {
            score -= 1000;
        }

        // control logic
        switch (c){
        case 'g':
            dots_on = !dots_on;
            draw();
            continue;
            break;
        case 't': // tutorial skip
            if (level == 0){ // should be level == 0
                level++;
                timer_on = -1;
                cha_x = S_WIDTH / 2;
                cha_y = S_HEIGHT / 2;

                do_gen_next_level();
                continue;
            }
            break;
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
                level != 7 &&
                level != 8 &&
                level != 9) { // activates ten turn shield for a score cost of 5
                shield = 10;
                score -= 5;
            }
            break;
        case 'e': // uses a grenade with a cost of 5
            if (score > 10 &&
                level != 7 &&
                level != 8 &&
                level != 9) {
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
        default:
            update_object(0, 0, 0);
            break;
        }

        for (unsigned int i = 0; i < enemies.size(); i++){
            tick_enemy(enemies[i], entities, cha_x, cha_y);
        }

        const sf::Texture& texture = renderTexture.getTexture();
        sf::Sprite sprite(texture);
        window.draw(sprite);
        window.clear();
        window.draw(shape);
        window.display();

        // draw screen
        draw();

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
    if (dots_on) paint_grid();

    // draw tutorial stuff
    if (level == 0){
        draw_level_text();
    }

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
            set_color(color_t::BRIGHT_RED);
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
        case 100:
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
    std::cout << " LEVEL:" << level;

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

void draw_title(void){
    jump_xy(0, 0); std::cout << "Version 0.1.0015-g";
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
    jump_xy(49, 19); std::cout << "Press space to begin";

    if (lvl == 7 ||
        lvl == 8 ||
        lvl == 9) {
        set_color(color_t::BRIGHT_RED);
        jump_xy(37,21);
        std::cout << "Warning: Bombs and grenades do not work here";
    }

    while (get_key() != ' ');

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
    jump_xy(x, y + 6); std::cout << "                   Score: " << global_score << NL;

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
    jump_xy(x, y + 6); std::cout << "                   Score: " << global_score << NL;

    get_key();

    exit(0);

}

void draw_level_text(){
    jump_xy(0, 2);
    std::cout << "Press 't' to skip this tutorial";

    if (cha_x > 0) {
        jump_xy(0,9);
        std::cout << "Use WASD to move";
    }
    if (cha_x > 25) {
        jump_xy(20,10);
        std::cout << "Avoid walls '#' ";
    }
    if (cha_x > 45) {
        jump_xy(40,11);
        std::cout << "Collect '+' tokens for +1 score";
        jump_xy(45,12);
        std::cout << "Collect '";
        set_color(color_t::YELLOW);
        std::cout << "$";
        set_color(color_t::NORMAL);
        std::cout << "' tokens for +5 score";
    }
    if (cha_x > 55) {
        jump_xy(50,16);
        std::cout << "Avoid '";
        set_color(color_t::BRIGHT_RED);
        std::cout << "!";
        set_color(color_t::NORMAL);
        std::cout << "' bombs. They have a radius of 2.";
        jump_xy(55,17);
        std::cout << "They do damage of -10 to your score.";
    }
    if (cha_x > 65) {
        jump_xy(60,20);
        std::cout << "Enemies '";
        set_color(color_t::LT_BLUE);
        std::cout << "O";
        set_color(color_t::NORMAL);
        std::cout << "' subtract 200 from your score.";
    }

    if (cha_x > 75) {
        jump_xy(0, 4);
        std::cout << "Press 'g' to turn on and off dots in the background. This does not effect gameplay.";
        jump_xy(0, 5);
        std::cout << "Press 'q' to activate your shield and protect yourself from bombs (only works if your score is above ten).";
        jump_xy(0, 6);
        std::cout << "Press 'e' to use a grenade on surrounding enemies (only works if your score is above five).";
        jump_xy(0, 24);
        std::cout << "Doors '";
        set_color(color_t::GR_GREEN);
        std::cout << "D";
        set_color(color_t::NORMAL);
        std::cout << "' take you to the next level.";
        jump_xy(0, 25);
        std::cout << "Game over occurs when your score is less than -20. Don't let it get that low.";jump_xy(0, 25);
        jump_xy(0, 27);
        set_color(color_t::DEAD);
        std::cout << "Good night, and good luck.";
        set_color(color_t::NORMAL);
    }
}
