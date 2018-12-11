#include <iostream>
#include "console.h"
#include "main.h"
#include "interact.h"
#include "draw.h"
#include "level.h"

void do_level_screen(int lvl){

}

void do_win_screen(void){
    state = 5;
}

void do_death_screen(void){
    state = 4;
}

bool respawn_entity(int idx){
    if (entities[idx].t == 5) return false;

    if (!respawn_bomb_on && entities[idx].t == 2) {
        entities[idx].x = -100;
        entities[idx].y = -100;
        return true;
    }

    if (regen_on){
        // TODO edit registration
        int x = rand() % S_WIDTH;
        int y = rand() % S_HEIGHT;
        while (abs(x - cha_x) <= 4 ||
               abs(y - cha_y) <= 4) {
            x = rand() % S_WIDTH;
            y = rand() % S_HEIGHT;
        }

        entities[idx].x = x; //x;
        entities[idx].y = y; //y;
    } else {
        entities[idx].x = -100;
        entities[idx].y = -100;
    }
    return true;
}

bool make_entity_at(int x, int y, int t){
    if (entity_spawn_lock &&
        abs(x - cha_x) <= 2 &&
        abs(y - cha_y) <= 2 &&
        (t == 2 || t == 4)) return false;

    if ((signed int)entities.size() - 5 >= S_WIDTH * S_HEIGHT)
        std::cout << "potential issues with placing entity at (" << x << "," << y << ") with type " << t << " (" << (signed int) entities.size() - 5 << " > " << S_WIDTH * S_HEIGHT << ")." << std::endl;

    entity_overlap_check_on = true;
    if (entity_overlap_check_on){
        for (unsigned int i = 0; i < registry.size(); i++){
            int x1 = -1;
            int y1 = -1;
            get_registry_xy(i, x1, y1);

            if (x1 == x && y1 == y) {
                std::cout << "    cancelled placing entity at (" << x << "," << y << ") with type " << t << std::endl;
                return false;
            }
        }
    }

    entity_t e;
    e.x = x;
    e.y = y;
    e.t = t;
    entities.push_back(e);
    register_object(global_uuid_next++, 0 /*entity*/, entities.size() - 1, t);
    return true;
}

bool player_set_safe(void){
    // go to safe space
    for (unsigned int i = 0; i < entities.size(); i++){
        if (abs(entities[i].x - cha_x) <= 3 && abs(entities[i].y - cha_y) <= 3){
            if (entities[i].t == 2) respawn_entity(i);
        }
    }

    return true;
}

void tick_enemy(enemy_t &en, std::vector<entity_t> &e, int cx, int cy){
    if (en._state < 0) return;
    if (en._score < -20 && en._t != 4) return;
    if (en._score < -50 && en._t == 4) do_win_screen(); // victory

    int dx = 0;
    int dy = 0;

    if (en._t == 0) { // randomwalks
        dx = rand() % 3 - 1;
        dy = rand() % 3 - 1;
    } else if (en._t == 1) { // hunter
        // TODO obj avoid
        dx = (en._x > cx) ? 1 : ((en._x == cx) ? 0 : -1 );
        dy = (en._y > cy) ? 1 : ((en._y == cy) ? 0 : -1 );
    } else if (en._t == 2) {
        en._state--;
        if (en._state == 0) {
            en._score += 30;
            damage_object_x(en._x, 30);
            en._state = 3 + rand() % 10;
        } else if (en._state == 1) {
            horizontal_beam(en._x);
        }
    } else if (en._t == 3) {
        en._state --;
        if (en._state == 0) {
            en._score += 30;
            damage_object_y(en._y, 30);
            en._state = 3 + rand() % 10;
        } else if (en._state == 1) {
            vertical_beam(en._y);
        }
    } else if (en._t == 4) {
        // BOSS ENEMY
        // hunts, lasers every other turn
        // killable basically only through grrnades
        // hunts at half speed
        // on death, spawns doors

        if (rand() % 2 == 0){
            dx = (en._x > cx) ? 1 : ((en._x == cx) ? 0 : -1 );
            dy = (en._y > cy) ? 1 : ((en._y == cy) ? 0 : -1 );

            dx = -1 * dx;
            dy = -1 * dy;
        }

        if (en._state == 0) {
            en._score += 100;
            damage_object_x(en._x, 30);
            damage_object_y(en._y, 30);
            en._state = 2;
        } else {
            en._state--;
        }
    }

    update_object(en._id, dx, dy);

    // score check
    if (en._score < -20 && en._t != 4) en._state = -1000;
    if (en._score < -50 && en._t == 4) {
        // spawn the door
        make_entity_at(S_WIDTH / 2, S_HEIGHT / 2, 100);
        en._state = -1000;
    }

    if (en._state < 0 && (en._t == 2 || en._t == 3)) {
        en._score = 10000;
        en._state = rand() % 10;
    }
}

void do_gen_next_level(void){
    registry.clear();
    enemies.clear();
    particles.clear();
    entities.clear();
    global_uuid_next = 0;

    global_score += score;
    score = 5;

    register_object(global_uuid_next++, 2, 0, 0);

    // reset flags
    respawn_bomb_on = true;

    switch(level){
    case 0:
    case 1:
    case 2:
        generate_terrain();
//        generate_gridworld();
        break;
    case 3:
        generate_safe_run();
        break;
    case 4:
    case 5:
        generate_lasers();
        break;
    case 6:
        generate_pacman();
        break;
    case 7:
    case 8:
    case 9:
        generate_dense_terrain();
        break;
    case 10:
        //generate_boss();
        generate_pacman_time();
        break;
    case 11:
    case 12:
        generate_gridworld();
        break;    make_entity_at(0, 0, 5);
    make_entity_at(S_WIDTH, 0, 5);
    make_entity_at(S_WIDTH, S_HEIGHT, 5);
    make_entity_at(0, S_HEIGHT, 5);
    case 13:
        generate_impossible();
        break;
    case 14:
        generate_gridworld();
        break;
    case 15:
        generate_boss();
        break;
    default:
        generate_empty();
        break;
    }

    cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    player_set_safe(); // go to safe location

    do_level_screen(level);

    //draw();
}
