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

/*
    types: 0, 1 -> normal
           2 -> bomb
           3 -> bonus
           4 -> wall
*/

// TODO add levels and level 10 = win

// globals lol
int cha_x = S_WIDTH / 2;
int cha_y = S_HEIGHT / 2;
int score = 5;
int shield = -1002; // signals first turn
std::vector<entity_t> entities;
std::vector<enemy_t> enemies;
int global_uuid_next = 0;

int main(){
    // configure random
    srand( time( NULL) );

    // print start screen
    set_color(color_t::GREEN);
    jump_xy(39,9); std::cout << "                                          " << ((char) 10);
    jump_xy(39,10); std::cout << "   __ _        __ _  __ _ _ __ ___   ___  " << ((char) 10);
    jump_xy(39,11); std::cout << "  / _` |_____ / _\\`|/ _` | '_ ` _ \\ / _ \\ " << ((char) 10);
    jump_xy(39,12); std::cout << " | (_| |_____| (_| | (_| | | | | | |  __/ " << ((char) 10);
    jump_xy(39,13); std::cout << "  \\__,_|      \\__, |\\__,_|_| |_| |_|\\___| " << ((char) 10);
    jump_xy(39,14); std::cout << "              |___/                       " << ((char) 10);
    jump_xy(39,15); std::cout << "                                          " << ((char) 10);
    set_color(color_t::NORMAL);
    jump_xy(39,16); std::cout << "              l e v e l   "; set_color(color_t::DARK_RED); std::cout << "# 1             " << ((char) 10);
    set_color(color_t::NORMAL);
    jump_xy(39,18); std::cout << "       Press any key to begin a game      " << std::endl;

    // allocate memory in vectors to avoid issues
    init_registry();
    entities.reserve(100);
    enemies.reserve(30);

    register_object(0, 2, 0, 0); // register player

    generate_terrain(); // generate terrain

    player_set_safe(); // go to safe location

    while (true) {
        char c = get_key(); // get input

        if (shield > 0) shield--; // decrease shield

        // control logic
        switch (c){
        case 'w':
            //cha_y--;
            update_object(0, 0, -1);
            break;
        case 'd':
            //cha_x++;
            update_object(0, 1, 0);
            break;
        case 's':
            //cha_y++;
            update_object(0, 0, 1);
            break;
        case 'a':
            //cha_x--;
            update_object(0, -1, 0);
            break;
        case 'q':
            if (score > 10) { // activates ten turn shield for a score cost of 5
                shield = 10;
                score -= 5;
            }
            break;
        case 'e': // uses a grenade with a cost of 5
            if (score > 10) {
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
        default:
            break;
        }

        // draw screen
        draw();

        for (int i = 0; i < enemies.size(); i++){
            if (enemies[i]._score < -20) continue;
            if (enemies[i]._state < 0) continue;
            enemy_t e = enemies[i];
            tick_enemy(e, entities, cha_x, cha_y);
            enemies[i] = e;
        }

        // first 3 turns are immune to score changes
        if (shield <= -1000){
            shield = 0;
            score = 5;
        }

        // game over check
        if (score < -20) {
            //clear_screen();
            //std::cout << "Game over";
            //return 0;
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
            std::cout << "#";  break;
        default:
            break;
        }
    }

    // enemies
    for (enemy_t e : enemies){
        jump_xy(e._x, e._y + 2);
        if (e._score >= -20) set_color(color_t::LT_BLUE);
        if (e._score < -20) set_color(color_t::DEAD);
        std::cout << "O";
        set_color(color_t::NORMAL);
    }

    // score
    jump_xy(0, 0);
    std::cout << "SCORE: " << score;

    // debug
    jump_xy(10, 0);
    //std::cout << registry.size() << "," << entities.size() << " | " << cha_x << "," << cha_y << " | " << S_WIDTH << "," << S_HEIGHT;

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
