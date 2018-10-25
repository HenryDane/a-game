#include <vector>
#include <iostream>
#include "windows.h"
#include "patch.h"
#include "main.h"
#include "interact.h"
#include "console.h"
#include "game.h"

std::vector<registry_key_t> registry;

bool init_registry (void){
    registry.clear();
    registry.reserve(200);
}

bool register_object(int id, int type, int ridx, int rtype){
    registry_key_t rk;
    rk.id = id;
    rk.type = type;
    rk.ridx = ridx;
    rk.rtype = rtype;
    registry.push_back(rk);
}

bool get_registry_xy(int id, int &x, int &y){
    switch (registry[id].type){
    case 0:
        x = entities[registry[id].ridx].x;
        y = entities[registry[id].ridx].y;
        break;
    case 1:
        x = enemies[registry[id].ridx]._x;
        y = enemies[registry[id].ridx]._y;
        break;
    case 2:
        x = cha_x;
        y = cha_y;
        break;
    }

    return true;
}

bool set_registry_xy(int id, int x, int y){
    switch (registry[id].type){
    case 0:
        entities[registry[id].ridx].x = x;
        entities[registry[id].ridx].y = y;
        break;
    case 1:
        enemies[registry[id].ridx]._x = x;
        enemies[registry[id].ridx]._y = y;
        break;
    case 2:
        cha_x = x;
        cha_y = y;
        break;
    }

    return true;
}

/*
takes the objects registry id, and its planned movement, and checks for issues
*/
bool update_object (int id, int dx, int dy){
    int x = -1;
    int y = -1;
    if (id > 0) get_registry_xy(id, x, y);
    if (id == 0) {
        x = cha_x;
        y = cha_y;
    }

    x += dx;
    y += dy;

    // border check
    if (x > S_WIDTH ||
        x < 0 ||
        y > S_HEIGHT ||
        y < 0){
        return false;
    }

    // wall check
    for (int i = 0; i < entities.size(); i++){
        if (entities[i].x == x && entities[i].y == y && entities[i].t == 4){ // walls
            return false;
        }
    }

    // approve change
    set_registry_xy(id, x, y);

    // interaction check
    for (int i = 0; i < registry.size(); i++){
        if (i == id) continue; // no self-interaction

        int x1 = -1; int y1 = -1; get_registry_xy(i, x1, y1); // get xy values of object

        if (x == x1 && y == y1) { // exact colission
            if (registry[i].type == 0 && (registry[i].rtype == 1 || registry[i].rtype == 0)){ // 0 or 1 entity
                if (registry[id].type == 2) score++;
                if (registry[id].type == 1) enemies[registry[id].ridx]._score++;
                respawn_entity(registry[i].ridx);
            } else if (registry[i].type == 0 && registry[i].rtype == 3) { // bonus coin
                if (registry[id].type == 2) score += 5;
                if (registry[id].type == 1) enemies[registry[id].ridx]._score += 5;
                respawn_entity(registry[i].ridx);
            } else if (registry[i].type == 1) {
                if (registry[id].type == 2) score -= 200;
                if (registry[id].type == 1) enemies[registry[id].ridx]._score -= 200;
            } else if (registry[i].type == 0 && registry[i].rtype == 5) {// exit
                level++;
                do_gen_next_level();
            }
        } else if (abs(x - x1) <= 2 && abs(y - y1) <= 2){ // radius = 2
            if (registry[i].type == 0 && registry[i].rtype == 2){ // bomb
                if (registry[id].type == 2 && shield == 0) score -= 10;
                if (registry[id].type == 1) enemies[registry[id].ridx]._score -= 10;
                draw_explosion(x1, y1, 2);
                respawn_entity(registry[i].ridx);
                get_key(); // wait for react
            }
        }
    }
}