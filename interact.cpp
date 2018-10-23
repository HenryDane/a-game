#include <vector>
#include "main.h"

struct registry_key_t {
    int id;
    int ridx;
    int type; // 0 = entity, 1 = enemy, 2 = character
    int rtype;
};

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
    get_registry_xy(id, x, y);

    // border check
    if (x + dx > S_WIDTH ||
        x + dx < 0 ||
        y + dy > S_HEIGHT ||
        y + dy < 0)
        return false;

    // wall check
    for (int i = 0; i < entities.size(); i++){
        if (entities[i].x == x + dx && entities[i].y == y + dy && entities[i].t == 4){ // walls
            return false;
        }
    }

    // approve change
    x += dx;
    y += dy;
    set_registry_xy(id, x, y);
}
