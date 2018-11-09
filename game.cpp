#include <iostream>
#include "console.h"
#include "main.h"
#include "interact.h"
#include "draw.h"

void do_level_screen(int lvl){

}

void do_win_screen(void){

}

void do_death_screen(void){
    state = 4;
}

bool respawn_entity(int idx){
    if (entities[idx].t == 5) return false;

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

    if (entity_overlap_check_on){
        for (unsigned int i = 0; i < registry.size(); i++){
            int x1 = -1;
            int y1 = -1;
            get_registry_xy(i, x1, y1);

            if (x1 == x && y1 == y) return false;
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

bool generate_empty(void){
    regen_on = true;
    timer_on = -1;

    entities.clear();
    enemies.clear();

    return true;
}

bool generate_tutorial(void){
    regen_on = false;
    timer_on = -1;

    entities.clear();
    enemies.clear();

    cha_x = 2; cha_y = 12;

    make_entity_at(30, 12 + 1, 4 );
    make_entity_at(30, 12 - 1, 4 );

    make_entity_at(50, 12, 1);

    make_entity_at(55, 12, 3);
    make_entity_at(56, 12, 3);

    make_entity_at(60, 12, 2);

    make_entity_at(69, 12, 4);
    enemy_t enemy;
    enemy.init(70, 12, -1, global_uuid_next++);
    enemies.push_back(enemy);
    register_object(enemy._id, 1, 0, -1);

    make_entity_at(80, 12, 5);

    return true;
}

bool generate_lasers(void) {
    regen_on = true;

    entities.clear();
    enemies.clear();

    for (int i = 0; i < 60; i++){
        make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 4);
    }

    for (int i = 0; i < 20; i++){
        enemy_t e;
        e.init(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 2 + 2, global_uuid_next++);
        enemies.push_back(e);
        e._score = 100000;
        register_object(e._id, 1 /*entity*/, i, 0);
    }

    make_entity_at(0, 0, 5);
    make_entity_at(S_WIDTH, 0, 5);
    make_entity_at(S_WIDTH, S_HEIGHT, 5);
    make_entity_at(0, S_HEIGHT, 5);

    return true;
}

bool generate_pacman(void) {
    //cha_x = S_WIDTH / 2;
    //cha_y = S_HEIGHT / 2;
    entities.clear();
    enemies.clear();
    regen_on = false;

    entity_spawn_lock = true;

    for (int i = 0; i < S_WIDTH + 1; i++){
        for (int j = 0; j < S_HEIGHT + 1; j++){
            if ((i == 0 && j == 0) ||
                (i == S_WIDTH && j == 0 ) ||
                (i == S_WIDTH && j == S_HEIGHT) ||
                (i == 0 && j == S_HEIGHT)) continue;

            int k = rand() % 10;
            if (k == 0 || k == 1){
                make_entity_at(i, j, 4);
            } else if (k == 2 || k == 3 || k == 4) {
                make_entity_at(i, j, 2);
            } else {
                make_entity_at(i, j, (rand() % 3 == 0) ? 3 : 1);
            }
        }
    }

    make_entity_at(0, 0, 5);
    make_entity_at(S_WIDTH, 0, 5);
    make_entity_at(S_WIDTH, S_HEIGHT, 5);
    make_entity_at(0, S_HEIGHT, 5);

    entity_spawn_lock = false;

    return true;
}

bool generate_terrain( void ){
    regen_on = true;
    timer_on = -10;
    cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    entities.clear();
    enemies.clear();

    for (int i = 0; i < 60; i++){
        make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 4);
    }

    for (int i = 0; i < 10; i++){
        enemy_t e;
        e.init(rand() % S_WIDTH, rand() % S_HEIGHT, 0, global_uuid_next++);
        enemies.push_back(e);
        register_object(e._id, 1 /*entity*/, i, 0);
    }

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

    make_entity_at(0, 0, 5);
    make_entity_at(S_WIDTH, 0, 5);
    make_entity_at(S_WIDTH, S_HEIGHT, 5);
    make_entity_at(0, S_HEIGHT, 5);

    return true;
}

bool generate_dense_terrain( void ){
    regen_on = true;
    timer_on = -10;
    cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    entities.clear();
    enemies.clear();

    entity_overlap_check_on = true;

    /*for (int i = 0; i < 100; i++){
        make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 4);
    }*/

    int ent_spawn = 0;
    while (ent_spawn < 100){
        ent_spawn += (make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 4) ? 1 : 0);
    }

    for (int i = 0; i < 20; i++){
        enemy_t e;
        e.init(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 2 + 2, global_uuid_next++);
        enemies.push_back(e);
        e._score = 100000;
        register_object(e._id, 1 /*entity*/, i, 0);
    }

    for (int i = 0; i < 10; i++){
        enemy_t e;
        e.init(rand() % S_WIDTH, rand() % S_HEIGHT, 0, global_uuid_next++);
        enemies.push_back(e);
        register_object(e._id, 1 /*entity*/, i, 0);
    }

    for (int i = 0; i < 50; i++){
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

    entity_overlap_check_on = false;

    make_entity_at(0, 0, 5);
    make_entity_at(S_WIDTH, 0, 5);
    make_entity_at(S_WIDTH, S_HEIGHT, 5);
    make_entity_at(0, S_HEIGHT, 5);

    return true;
}

bool generate_safe_run(void) {
    generate_terrain();

    regen_on = true;
    timer_on = 35;

    for (int i = 0; i < 60; i++){
        make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, 6);
    }

    return true;
}

bool generate_boss(void){
    cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    entities.clear();
    enemies.clear();

    regen_on = false;
    timer_on = 50;

    for (int i = 0; i < 50; i++){
        make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, 3);
        make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, 2);
    }

    make_entity_at(cha_x - 1, cha_y - 1, 3);
    make_entity_at(cha_x, cha_y - 1, 3);
    make_entity_at(cha_x + 1, cha_y - 1, 3);
    make_entity_at(cha_x - 1, cha_y + 1, 3);
    make_entity_at(cha_x, cha_y + 1, 3);
    make_entity_at(cha_x + 1, cha_y + 1, 3);
    make_entity_at(cha_x - 1, cha_y, 3);
    make_entity_at(cha_x + 1, cha_y, 3);

    // ADD BOSS ENEMY
    enemy_t boss;
    boss.init(rand() % S_WIDTH, rand() % S_HEIGHT, 4, global_uuid_next++);
    boss._score = 50;
    enemies.push_back(boss);
    register_object(boss._id, 1 /*entity*/, 0, 4);

    for(int i = 0; i < 10; i++){
        enemy_t e;
        e.init(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 2 + 2, global_uuid_next++);
        e._score = 100000;
        enemies.push_back(e);
        register_object(e._id, 1, i + 1, e._t);
    }

    return true;
}

bool player_set_safe(void){
    // go to safe space
    for (unsigned int i = 0; i < entities.size(); i++){
        if (abs(entities[i].x - cha_x) <= 3 && abs(entities[i].y < cha_y) <= 3){
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
        }
    } else if (en._t == 3) {
        en._state --;
        if (en._state == 0) {
            en._score += 30;
            damage_object_y(en._y, 30);
            en._state = 3 + rand() % 10;
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
    global_uuid_next = 0;

    global_score += score;
    score = 5;

    register_object(global_uuid_next++, 2, 0, 0);

    switch(level){
    case 0:
    case 1:
    case 2:
        generate_terrain();
        break;
    case 3:
        //generate_pacman();
        generate_safe_run();
        break;
    case 4:
    case 5:
        generate_lasers();
        break;
    case 6:
        //generate_safe_run();
        generate_pacman();
        break;
    case 7:
    case 8:
    case 9:
        generate_dense_terrain();
        break;
    case 10:
        generate_boss();
        break;
    default:
        generate_empty();
        break;
    }

    cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    player_set_safe(); // go to safe location

    clear_screen();
    do_level_screen(level);

    //draw();
}

