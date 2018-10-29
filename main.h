#include <vector>
#include <stdlib.h>

#define S_WIDTH 100
#define S_HEIGHT 25

#define __S(x) patch::to_string(x)

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

extern int cha_x;
extern int cha_y;
extern int score;
extern int global_score;
extern int shield; // signals first turn
extern std::vector<entity_t> entities;
extern std::vector<enemy_t> enemies;

extern int level;

extern int global_uuid_next;

extern bool regen_on;
extern int timer_on;
extern bool entity_spawn_lock;
extern bool entity_overlap_check_on;

// prototypes
void draw(void);
void draw_explosion(int x, int y, int r);
void draw_level_screen(int lvl);
void do_win_screen();
void do_death_screen();
void draw_level_text();
