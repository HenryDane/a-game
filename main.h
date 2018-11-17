#include <vector>
#include <stdlib.h>
#include <SFML/Graphics.hpp>

#define S_WIDTH 80
#define S_HEIGHT 36

#define TEX_SIZE 128
#define TEX_SCALE .125
#define TILE_WIDTH (TEX_SIZE * TEX_SCALE)
#define HALF_TILE_WIDTH (TILE_WIDTH / 2)

#define __S(x) patch::to_string(x)

/*
enum BitLookup {
    BIT0 = 1 << 0,
    BIT1 = 1 << 1,
    BIT2 = 1 << 2,
    BIT3 = 1 << 3,
    BIT4 = 1 << 4,
    BIT5 = 1 << 5,
    BIT6 = 1 << 6,
    BIT7 = 1 << 7
};
*/

struct entity_t {
    int x;
    int y;
    int t; // 0, 1 -> normal, 2 -> bomb, 3 -> coin, 4 -> wall, 5 -> exit, 6 -> timeplus
};

struct enemy_t {
    int _x;
    int _y;
    int _t; // 0 -> randomwalk, 1 -> hunter, 2 -> v laser, 3 -> h laser
    int _score;
    int _state;
    int _id;

    void init(int x, int y, int t, int id) {
        _x = x;
        _y = y;
        _t = t;
        if (_t == 2 || _t == 3) {
            _state = rand() % 10;
        } else {
            _state = 0;
        }
        _score = 20;
        _id = id;
    }
};

struct particle_t {
    int x;
    int y;
    int ttl;
    int type; // 0 -> fire, 1 -> v laser, 2 -> h laser
};

extern int cha_x;
extern int cha_y;
extern int score;
extern int global_score;
extern int shield; // signals first turn
extern std::vector<entity_t> entities;
extern std::vector<enemy_t> enemies;
extern std::vector<particle_t> particles;

extern int level;

extern int state;

extern int global_uuid_next;

extern bool regen_on;
extern int timer_on;
extern bool entity_spawn_lock;
extern bool entity_overlap_check_on;
extern bool respawn_bomb_on;

int handle_key(sf::Keyboard::Key k);

extern sf::RenderTexture renderTexture;

extern char __NL;
