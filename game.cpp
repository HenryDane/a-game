#include "main.h"

bool respawn_entity(int idx){
    int x = rand() % S_WIDTH;
    int y = rand() % S_HEIGHT;
    while (abs(x - cha_x) <= 4 ||
           abs(y - cha_y) <= 4) {
        x = rand() % S_WIDTH;
        y = rand() % S_HEIGHT;
    }

    entities[idx].t = rand() % 4;
    entities[idx].x = x;
    entities[idx].y = y;
    return true;
}

bool make_entity_at(int x, int y, int t){
    entity_t e;
    e.x = x;
    e.y = y;
    e.t = t;
    entities.push_back(e);
    return true;
}

bool generate_terrain( void ){
    entities.clear();
    enemies.clear();

    for (int i = 0; i < 20; i++){
        int x = rand() % S_WIDTH;
        int y = rand() % S_HEIGHT;
        switch(rand() % 4){
        case 0: // L block
            make_entity_at(x, y, 4);
            make_entity_at(x + 1, y, 4);
            make_entity_at(x + 1, y + 1, 4);
            make_entity_at(x + 1, y + 2, 4);
            break;
        case 1: // make u block
            make_entity_at(x, y, 4);
            make_entity_at(x + 1, y, 4);
            make_entity_at(x + 2, y, 4);
            make_entity_at(x + 2, y + 1, 4);
            make_entity_at(x + 2, y + 2, 4);
            make_entity_at(x, y + 1, 4);
            make_entity_at(x, y + 2, 4);
            break;
        case 2: // make ii block
            make_entity_at(x, y + 1, 4);
            make_entity_at(x, y + 2, 4);
            make_entity_at(x + 2, y, 4);
            make_entity_at(x + 2, y + 1, 4);
            break;
        case 3:
            make_entity_at(x, y, 4);
            make_entity_at(x + 2, y + 2, 4);
            make_entity_at(x, y + 2, 4);
            make_entity_at(x + 2, y, 4);
            break;
        default:
            break;
        }
    }

    /*
    for (int i = 0; i < 60; i++){
        make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 4);
    }

    for (int i = 0; i < entities.size(); i++){
        if (entities[i].t == 2) respawn_entity(i);
    }

    for (int i = 0; i < 10; i++){
        enemy_t e;
        e.init(rand() % S_WIDTH, rand() % S_HEIGHT, 0);
        enemies.push_back(e);
    }*/

    // clean up entities
    for (int i = 0; i < entities.size(); i++){
        if (entities[i].x >= S_WIDTH || entities[i].x < 0 ||
            entities[i].y >= S_HEIGHT || entities[i].y < 0) {
            entities.erase(entities.begin() + i);
            i--;
        }
    }

    return true;
}

bool player_set_safe(void){
    // go to safe space
    for (int i = 0; i < entities.size(); i++){
        if (abs(entities[i].x - cha_x) <= 3 || abs(entities[i].y < cha_y) <= 3){
            if (entities[i].t == 2) respawn_entity(i);
        }
    }

}
