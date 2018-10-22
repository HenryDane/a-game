#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <ctime>

#include "console.h"

#define S_WIDTH 50
#define S_HEIGHT 25

/*
    TODO
    -   add enemies which hunt and also avoid damage
    -   add horiz/vert weapons
    -   add particles
    -   add win condition
*/

struct entity_t {
    int x;
    int y;
    int t;
};

struct enemy_t {
    int _x;
    int _y;
    int _t;
    int _score;
    int _state;

    void init(int x, int y, int t) {
        _x = x;
        _y = y;
        _t = t;
        _state = 0;
        _score = 5;
    }

    void tick(std::vector<entity_t> &e, int cx, int cy){
        if (_t == 0){
            // randomwalks
            _x += rand() % 3 - 1;
            _y += rand() % 3 - 1;
        } else if (_t == 1){ // hunter
            // TODO obj avoid
            _x -= (_x > cx) ? 1 : ((_x == cx) ? 0 : -1 );
            _y -= (_y > cy) ? 1 : ((_y == cy) ? 0 : -1 );
        }

        // boundary check
        if (_x < 0) _x++;
        if (_y < 0) _y++;
        if (_x > S_WIDTH) _x--;
        if (_y > S_HEIGHT) _y--;

        // score check
        if (_score < -20) _state = -1;
    }
};

// globals lol
int cha_x = 5;
int cha_y = 5;
int score = 5;
int shield = -1002; // signals first turn
std::vector<entity_t> entities;
std::vector<enemy_t> enemies;

void draw(void);

int main(){
    // configure random
    srand( time( NULL) );

    std::cout << "                                          " << ((char) 10);
    std::cout << "   __ _        __ _  __ _ _ __ ___   ___  " << ((char) 10);
    std::cout << "  / _` |_____ / _\\`|/ _` | '_ ` _ \\ / _ \\ " << ((char) 10);
    std::cout << " | (_| |_____| (_| | (_| | | | | | |  __/ " << ((char) 10);
    std::cout << "  \\__,_|      \\__, |\\__,_|_| |_| |_|\\___| " << ((char) 10);
    std::cout << "              |___/                       " << ((char) 10);
    std::cout << "                                          " << ((char) 10);
    std::cout << "       Press any key to begin a game      " << std::endl;


    // allocate memory in vectors to avoid issues
    entities.reserve(40);
    enemies.reserve(15);

    // generate initial entities
    for(int i = 0; i < 35; i++){
        entity_t e;
        e.x = rand() % S_WIDTH;
        e.y = rand() % S_HEIGHT;
        e.t = rand() % 3;
        entities.push_back(e);
    }

    // generate initial enemies
    for (int i = 0; i < 10; i++){
        enemy_t e;
        e.init(rand() % S_WIDTH, rand() % S_HEIGHT, 0);
        enemies.push_back(e);
    }

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
                    jump_xy(10, 0);

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
            default:
                break;
            }
        }

        // boundary logic
        if (cha_x < 0 || cha_y < 0 || cha_x > S_WIDTH || cha_y > S_HEIGHT){
            if (cha_x < 0) cha_x++;
            if (cha_y < 0) cha_y++;
            if (cha_y >= S_HEIGHT) cha_y--;
            if (cha_x >= S_WIDTH) cha_x--;
        }

        // entitiy logic
        for (unsigned int i = 0; i < entities.size(); i++){
            if (entities[i].x == cha_x && entities[i].y == cha_y && (entities[i].t == 0 || entities[i].t == 1)) { // regular entities
                score++;
                entities[i].t = rand() % 3; // regen entity ....
                entities[i].x = rand() % S_WIDTH;
                entities[i].y = rand() % S_HEIGHT;
            } else if (abs(entities[i].x - cha_x) < 3 && abs(entities[i].y - cha_y) < 3 && entities[i].t == 2){ // bombs
                set_color(color_t::BRIGHT_RED);
                for (int k = -2; k <= 2; k++){
                    for (int m = -2; m <= 2; m++){
                        jump_xy(entities[i].x + k, entities[i].y + m + 2);
                        std::cout << "*";
                    }
                }
                set_color(color_t::DARK_RED);

                if (shield <= 0) score -= 10;

                jump_xy(entities[i].x, entities[i].y + 2);
                std::cout << "!";
                set_color(color_t::NORMAL);

                get_key();

                entities[i].t = rand() % 3;
                entities[i].x = rand() % S_WIDTH;
                entities[i].y = rand() % S_HEIGHT;
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
                if (entities[j].t == 1 && entities[j].x == enemies[i]._x && entities[j].y == enemies[i]._y){
                    enemies[i]._score--;
                    entities[j].x = rand() % S_WIDTH;
                    entities[j].y = rand() % S_HEIGHT;
                    entities[j].t = rand() % 3;
                } else if  ( abs(entities[j].x - enemies[i]._x) < 3 && abs(entities[j].y - enemies[i]._y) < 3 && entities[j].t == 2) {
                    enemies[i]._score -= 10; // score effect

                    // visual effect
                    set_color(color_t::BRIGHT_RED);
                    for (int k = -2; k <= 2; k++){
                        for (int m = -2; m <= 2; m++){
                            jump_xy(entities[j].x + k, entities[j].y + m + 2);
                            std::cout << "*";
                        }
                    }
                    set_color(color_t::DARK_RED);
                    jump_xy(entities[j].x, entities[j].y + 2);
                    std::cout << "!";
                    set_color(color_t::NORMAL);

                    entities[j].t = rand() % 3;
                    entities[j].x = rand() % S_WIDTH;
                    entities[j].y = rand() % S_HEIGHT;
                }
            }
        }

        draw();

        // first 3 turns are immune to score changes
        if (shield <= -1000){
            shield = 0;
            score = 5;
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
    jump_xy(0, 3 + S_HEIGHT);
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
        default:
            break;
        }
    }

    // enemies
    for (enemy_t e : enemies){
        jump_xy(e._x, e._y + 2);
        if (e._state >= 0) set_color(color_t::LT_BLUE);
        if (e._state < 0) set_color(color_t::DEAD);
        std::cout << "#";
        set_color(color_t::NORMAL);
    }

    // score
    jump_xy(0, 0);
    std::cout << score;

    // reset cursor
    jump_xy(0, 0);
}
