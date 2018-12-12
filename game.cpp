#include <iostream>
#include <fstream>
#include "patch.h"
#include "console.h"
#include "main.h"
#include "interact.h"
#include "draw.h"
#include "level.h"

void do_level_screen(int lvl){
    // do nothing
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
        break;
    case 13:
        generate_impossible();
        break;
    case 14:
        generate_gridworld();
        break;
    case 15:
    case 16:
    case 17:
        generate_terrain();
        break;
    case 19:
        generate_boss();
        break;
    default:
        generate_empty();
        break;
    }

    cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    player_set_safe(); // go to safe location

    // correct laser enemies
    for (int i = 0; i < enemies.size(); i++){
        if (enemies[i]._t != 2 && enemies[i]._t != 3) continue;

        if (enemies[i]._x == cha_x) enemies[i]._x--;
        if (enemies[i]._y == cha_y) enemies[i]._y++;
    }

    do_level_screen(level);

    //draw();
}

void do_new_game() {
    // clean up lists
    registry.clear();
    enemies.clear();
    entities.clear();
    particles.clear();

    // set up variables
    cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    score = 5;
    global_score = 0;
    level = 0;
    shield = -1002; // signals first turn
    turns = 0;

    // reset flags
    regen_on = true; // do entities regen?
    timer_on = -1; // if >= 0 timer is on
    // dots_on = false; // flag for dots display
    entity_spawn_lock = false; // flag for deleting everything nearby
    entity_overlap_check_on = false; // prevent overlap spawn
    respawn_bomb_on = true;

    // set up lists
    init_registry();
    entities.reserve(100);
    enemies.reserve(30);
    particles.reserve(100);

    // register player
    register_object(global_uuid_next++, 2, 0, 0); // register player

    // prepare map
    generate_tutorial(); // set up map

    // spawn player
    player_set_safe(); // go to safe location

    // begin actual game
    state = 2;
}

void load_game(int slot) {
    bool debug = false;

    // clean up lists
    registry.clear();
    enemies.clear();
    entities.clear();
    particles.clear();

    // set up variables
    cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    score = 5;
    global_score = 0;
    level = 0;
    shield = -1002; // signals first turn
    turns = 0;

    // reset flags
    regen_on = true; // do entities regen?
    timer_on = -1; // if >= 0 timer is on
    // dots_on = false; // flag for dots display
    entity_spawn_lock = false; // flag for deleting everything nearby
    entity_overlap_check_on = false; // prevent overlap spawn
    respawn_bomb_on = true;

    // set up lists
    init_registry();
    entities.reserve(100);
    enemies.reserve(30);
    particles.reserve(100);

    std::fstream file;
    file.open("slot" + patch::to_string(slot), std::ios::in);
    if (!file.is_open()) {
        std::cout << "Failed to load slot" << slot << "! Unable to load!" << std::endl;
        //abort();
        return;
    }

    std::string tmp;
    int t_size;
    int t_idx;

    file >> tmp;
    if (tmp[0] != '=') {
        std::cout << "Failed to read header" << std::endl;
        abort();
    }

    // read variables
    file >> cha_x >> cha_y >> score >> global_score >> level >> shield >> turns >> global_uuid_next;

    // read flags
    file >> regen_on >> timer_on >> entity_spawn_lock >> entity_overlap_check_on >> respawn_bomb_on;

    register_object(0, 2, 0, 0); // register player

    // read registry
    file >> t_size;
    for (int i = 0 ; i < t_size; i++){
        registry_key_t rkt_t;
        file >> t_idx >> rkt_t.id >> rkt_t.ridx >> rkt_t.rtype >> rkt_t.type;
        if (debug) std::cout << "Loaded registry key " << t_idx << std::endl;
        registry.push_back(rkt_t);
    }

    // read enemies
    file >> t_size;
    for (int i = 0 ; i < t_size; i++){
        enemy_t e_t;
        file >> t_idx >> e_t._id >> e_t._score >> e_t._state >> e_t._t >> e_t._x >> e_t._y;
        if (debug) std::cout << "Loaded enemy " << t_idx << std::endl;
        enemies.push_back(e_t);
    }

    // read entities
    file >> t_size;
    for (int i = 0 ; i < t_size; i++){
        entity_t e_t;
        file >> t_idx >> e_t.t >> e_t.x >> e_t.y;
        if (debug) std::cout << "Loaded entity " << t_idx << std::endl;
        entities.push_back(e_t);
    }

    // read particles
    file >> t_size;
    for (int i = 0 ; i < t_size; i++){
        particle_t p_t;
        file >> t_idx >> p_t.ttl >> p_t.type >> p_t.x >> p_t.y;
        if (debug) std::cout << "Loaded particle " << t_idx << std::endl;
        particles.push_back(p_t);
    }

    for (int i = 0; i < 4; i++) {
        file >> tmp;
        if (tmp != "[END]") {
            std::cout << "Bad footer" << std::endl;
            abort();
        }
    }

    file >> tmp;
    if (debug) std::cout << tmp;

    file.close();

    // PRINT
    if (debug) {
        std::cout << "=========SLOT" << slot << "=========" << std::endl;

        // write globals
        std::cout << cha_x << " " << cha_y << " " << score << " " << global_score << " " << level << " " << shield << " " << turns << " " << global_uuid_next << " " << std::endl;

        // write flags
        std::cout << regen_on << " " << timer_on << " " << entity_spawn_lock << " " << entity_overlap_check_on << " " << respawn_bomb_on << " " << std::endl;

        // write registry
        std::cout << registry.size() << std::endl;
        for (int i = 0; i < registry.size(); i++) {
            std::cout << i << " " << registry[i].id << " " << registry[i].ridx << " " << registry[i].rtype << " " << registry[i].type << " " << std::endl;
        }

        // write enemies
        std::cout << enemies.size() << std::endl;
        for (int i = 0; i < enemies.size(); i++) {
            std::cout << i << " " << enemies[i]._id << " " << enemies[i]._score << " " << enemies[i]._state << " " << enemies[i]._t << " " << enemies[i]._x << " " << enemies[i]._y << " " << std::endl;
        }

        // write entities
        std::cout << entities.size() << std::endl;
        for (int i = 0; i < entities.size(); i++) {
            std::cout << i << " " << entities[i].t << " " << entities[i].x << " " << entities[i].y << " " << std::endl;
        }

        // write particles
        std::cout << particles.size() << std::endl;
        for(int i = 0; i < particles.size(); i++){
            std::cout << i << " " << particles[i].ttl << " " << particles[i].type << " " << particles[i].x << " " << particles[i].y << " " << std::endl;
        }

        std::cout << "[END] [END] [END] [END]" << std::endl;

        std::cout << "=========SLOTEND=========" << std::endl;
    }

    std::cout << "Done loading." << std::endl;
}

void save_game(int slot) {
    bool debug = false;

    std::fstream file;
    file.open("slot" + patch::to_string(slot), std::ios::out);
    if (!file.is_open()) {
        std::cout << "Failed to load slot" << slot << "! Crashing...." << std::endl;
        abort();
    }

    // header
    file << "=========SLOT" << slot << "=========" << std::endl;
    if (debug) std::cout << "=========SLOT" << slot << "=========" << std::endl;

    // write globals
    file << cha_x << " " << cha_y << " " << score << " " << global_score << " " << level << " " << shield << " " << turns << " " << global_uuid_next << " " << std::endl;
    if (debug) std::cout << cha_x << " " << cha_y << " " << score << " " << global_score << " " << level << " " << shield << " " << turns << " " << global_uuid_next << " " << std::endl;

    // write flags
    file << regen_on << " " << timer_on << " " << entity_spawn_lock << " " << entity_overlap_check_on << " " << respawn_bomb_on << " " << std::endl;
    if (debug) std::cout << regen_on << " " << timer_on << " " << entity_spawn_lock << " " << entity_overlap_check_on << " " << respawn_bomb_on << " " << std::endl;

    // write registry
    file << registry.size() << std::endl;
    if (debug) std::cout << registry.size() << std::endl;
    for (int i = 0; i < registry.size(); i++) {
        file << i << " " << registry[i].id << " " << registry[i].ridx << " " << registry[i].rtype << " " << registry[i].type << " " << std::endl;
        if (debug) std::cout << i << " " << registry[i].id << " " << registry[i].ridx << " " << registry[i].rtype << " " << registry[i].type << " " << std::endl;
    }

    // write enemies
    file << enemies.size() << std::endl;
    if (debug) std::cout << enemies.size() << std::endl;
    for (int i = 0; i < enemies.size(); i++) {
        file << i << " " << enemies[i]._id << " " << enemies[i]._score << " " << enemies[i]._state << " " << enemies[i]._t << " " << enemies[i]._x << " " << enemies[i]._y << " " << std::endl;
        if (debug) std::cout << i << " " << enemies[i]._id << " " << enemies[i]._score << " " << enemies[i]._state << " " << enemies[i]._t << " " << enemies[i]._x << " " << enemies[i]._y << " " << std::endl;
    }

    // write entities
    file << entities.size() << std::endl;
    if (debug) std::cout << entities.size() << std::endl;
    for (int i = 0; i < entities.size(); i++) {
        file << i << " " << entities[i].t << " " << entities[i].x << " " << entities[i].y << " " << std::endl;
        if (debug) std::cout << i << " " << entities[i].t << " " << entities[i].x << " " << entities[i].y << " " << std::endl;
    }

    // write particles
    file << particles.size() << std::endl;
    if (debug) std::cout << particles.size() << std::endl;
    for(int i = 0; i < particles.size(); i++){
        file << i << " " << particles[i].ttl << " " << particles[i].type << " " << particles[i].x << " " << particles[i].y << " " << std::endl;
        if (debug) std::cout << i << " " << particles[i].ttl << " " << particles[i].type << " " << particles[i].x << " " << particles[i].y << " " << std::endl;
    }

    file << "[END] [END] [END] [END]" << std::endl;
    if (debug) std::cout << "[END] [END] [END] [END]" << std::endl;

    file << "=========SLOTEND=========" << std::endl;
    if (debug) std::cout << "=========SLOTEND=========" << std::endl;

    std::cout << "Done saving." << std::endl;

    file.close();
}

void goto_loaded_game() {
    state = 1;
}

void select_load_level(int lvl) {
    level = lvl;
    do_gen_next_level();
}
