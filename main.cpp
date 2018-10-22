#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cmath>
#include "console.h"

#define S_WIDTH 50
#define S_HEIGHT 25

struct entity_t {
    unsigned int x;
    unsigned int y;
    unsigned int t;
};

struct enemy_t {
    unsigned int _x;
    unsigned int _y;
    unsigned int _t;
    int state;

    void init(int x, int y, int t) {
        _x = x;
        _y = y;
        _t = t;
        state = 0;
    }

    void tick(std::vector<entity_t> &e, unsigned int cx, unsigned int cy){
        if (_t == 0){
            // randomwalks
            _x += rand() % 3 - 1;
            _y += rand() % 3 - 1;

            if (_x < 0) _x++;
            if (_y < 0) _y++;
            if (_x > S_WIDTH) _x--;
            if (_y > S_HEIGHT) _y--;
        } else if (_t == 1){ // hunter
            // TODO obj avoid
            _x -= (_x > cx) ? 1 : ((_x == cx) ? 0 : -1 );
            _y -= (_y > cy) ? 1 : ((_y == cy) ? 0 : -1 );
        }
    }
};

// globals lol
int cha_x = 5;
int cha_y = 5;
int score = 0;
int shield = 0;
std::vector<entity_t> entities;
std::vector<enemy_t> enemies;

void draw(void);

int main(){
    entities.reserve(40);
    enemies.reserve(15);

    for(int i = 0; i < 35; i++){
        entity_t e;
        e.x = rand() % S_WIDTH;
        e.y = rand() % S_HEIGHT;
        e.t = rand() % 3;
        entities.push_back(e);
    }
    for (int i = 0; i < 10; i++){
        enemy_t e;
        e.init(rand() % S_WIDTH, rand() % S_HEIGHT, 1);
        enemies.push_back(e);
    }

    score = 5; // reset score
    std::string c;
    while (true) {
        //std::cout << "DIR? ";
        //std::cin >> c;
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
                if (score > 10) {
                    shield = 10;
                    score -= 5;
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

        // game logic
        for (unsigned int i = 0; i < entities.size(); i++){
            // regular entities
            if (entities[i].x == cha_x && entities[i].y == cha_y && (entities[i].t == 0 || entities[i].t == 1)) {
                score++;
                entities[i].x = rand() % S_WIDTH;
                entities[i].y = rand() % S_HEIGHT;
            }

            // bombs
            if (abs(entities[i].x - cha_x) < 3 &&
                abs(entities[i].y - cha_y) < 3 &&
                entities[i].t == 2){
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
        for (int i = 0; i < enemies.size(); i++){
            if (enemies[i]._x == cha_x && enemies[i]._y == cha_y){
                score -= 20;
                jump_xy(0, 10);
                std::cout << "Enemy!";
            }
            enemies[i].tick(entities, cha_x, cha_y);
        }

        draw();

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
        case 3:
            std::cout << "@"; break;
        default:
            break;
        }
    }

    // enemies
    for (enemy_t e : enemies){
        jump_xy(e._x, e._y + 2);
        set_color(color_t::LT_BLUE);
        std::cout << "#";
        set_color(color_t::NORMAL);
    }

    // score
    jump_xy(0, 0);
    std::cout << score;

    // reset cursor
    jump_xy(0, 0);
}
