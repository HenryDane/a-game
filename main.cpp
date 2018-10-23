#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include "main.h"
#include "console.h"
#include "game.h"

/*
    types: 0, 1 -> normal
           2 -> bomb
           3 -> bonus
           4 -> wall
*/

// TODO why does wall generation only work on ';' key event??

// globals lol
int cha_x = S_WIDTH / 2;
int cha_y = S_HEIGHT / 2;
int score = 5;
int shield = -1002; // signals first turn
std::vector<entity_t> entities;
std::vector<enemy_t> enemies;

// prototypes
void draw(void);
void draw_explosion(int x, int y, int r);

int main(){
    // configure random
    srand( time( NULL) );

    set_color(color_t::GREEN);
    jump_xy(39,9); std::cout << "                                          " << ((char) 10);
    jump_xy(39,10); std::cout << "   __ _        __ _  __ _ _ __ ___   ___  " << ((char) 10);
    jump_xy(39,11); std::cout << "  / _` |_____ / _\\`|/ _` | '_ ` _ \\ / _ \\ " << ((char) 10);
    jump_xy(39,12); std::cout << " | (_| |_____| (_| | (_| | | | | | |  __/ " << ((char) 10);
    jump_xy(39,13); std::cout << "  \\__,_|      \\__, |\\__,_|_| |_| |_|\\___| " << ((char) 10);
    jump_xy(39,14); std::cout << "              |___/                       " << ((char) 10);
    jump_xy(39,15); std::cout << "                                          " << ((char) 10);
    set_color(color_t::NORMAL);
    jump_xy(39,16); std::cout << "       Press any key to begin a game      " << std::endl;


    // allocate memory in vectors to avoid issues
    entities.reserve(100);
    enemies.reserve(30);

    generate_terrain();

    player_set_safe();

    std::string c;
    while (true) {
        c = get_key();
        if (shield > 0) shield--;

        // control logic
        if (c.size() == 1) {
            char ci = c.at(0);

            switch (ci){
            case 'w':
                cha_y--; break;
            case 'd':
                cha_x++; break;
            case 's':
                cha_y++; break;
            case 'a':
                cha_x--; break;
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
        }

        // boundary logic
        if (cha_x < 0 || cha_y < 0 || cha_x > S_WIDTH || cha_y > S_HEIGHT){
            if (cha_x < 0) cha_x++;
            if (cha_y < 0) cha_y++;
            if (cha_y > S_HEIGHT) cha_y--;
            if (cha_x > S_WIDTH) cha_x--;
        }

        // entitiy logic
        for (unsigned int i = 0; i < entities.size(); i++){
            if (entities[i].x == cha_x && entities[i].y == cha_y && (entities[i].t == 0 || entities[i].t == 1)) { // regular entities
                score++;
                respawn_entity(i);
            } else if (entities[i].x == cha_x && entities[i].y == cha_y && (entities[i].t == 3)) {
                score += 5;
                respawn_entity(i);
            } else if (abs(entities[i].x - cha_x) < 3 && abs(entities[i].y - cha_y) < 3 && entities[i].t == 2){ // bombs
                draw_explosion(entities[i].x, entities[i].y, 2);
                set_color(color_t::DARK_RED);

                if (shield <= 0) score -= 10;

                jump_xy(entities[i].x, entities[i].y + 2);
                std::cout << "!";
                set_color(color_t::NORMAL);

                get_key();

                respawn_entity(i);
            }
        }
        // enemy logic
        for (unsigned int i = 0; i < enemies.size(); i++){
            if (enemies[i]._state < 0) continue;
            if (enemies[i]._x == cha_x && enemies[i]._y == cha_y){
                score -= 20;
                jump_xy(0, 10);
                std::cout << "Enemy!";
            }
            enemies[i].tick(entities, cha_x, cha_y);

            // enemy-entity check
            for (unsigned int j = 0; j < entities.size(); j++){
                if (entities[j].t == 1 && entities[j].x == enemies[i]._x && entities[j].y == enemies[i]._y && (entities[j].t == 0 || entities[j].t == 1)){
                    enemies[i]._score++;
                    respawn_entity(j);
                } else if (entities[j].t == 1 && entities[j].x == enemies[i]._x && entities[j].y == enemies[i]._y && (entities[j].t == 3)){
                    enemies[i]._score += 5;
                    respawn_entity(j);
                } else if  ( abs(entities[j].x - enemies[i]._x) < 3 && abs(entities[j].y - enemies[i]._y) < 3 && entities[j].t == 2) {
                    enemies[i]._score -= 10; // score effect
                    respawn_entity(j);
                }
            }
        }

        draw();

        // first 3 turns are immune to score changes
        if (shield <= -1000){
            shield = 0;
            score = 5;
        }

        // game over check
        if (score < -20) {
            clear_screen();
            std::cout << "Game over";
            return 0;
        }
    }
    return 0;
}

void draw(void){
    // draw logic
    clear_screen();

    // border
    jump_xy(0, 1);
    print_nchr(S_WIDTH + 1, '=');
    jump_xync(0, 3 + S_HEIGHT);
    print_nchr(S_WIDTH + 1, '=');

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
        if (e._state >= 0) set_color(color_t::LT_BLUE);
        if (e._state < 0) set_color(color_t::DEAD);
        std::cout << "O";
        set_color(color_t::NORMAL);
    }

    // score
    jump_xy(0, 0);
    std::cout << score;

    // debug
    jump_xy(10, 0);
    std::cout << cha_x << "," << cha_y << " | " << S_WIDTH << "," << S_HEIGHT;

    // reset cursor
    jump_xy(0, 0);
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
