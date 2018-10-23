#include "main.h"

bool respawn_entity(int idx){

}

bool make_entity_at(int x, int y, int t){
    entity_t e;
    e.x = x;
    e.y = y;
    e.t = t;
    entities.push_back(e);
}

bool generate_terrain( void ){
    for (int i = 0; i < 10; i++){
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

    // clean up entities
    for (int i = 0; i < entities.size())
}

bool player_set_safe(void){
    // go to safe space

}
