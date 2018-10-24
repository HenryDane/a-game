#include <vector>
#include <stdlib.h>

#define S_WIDTH 100
#define S_HEIGHT 25

#define __S(x) patch::to_string(x)

struct entity_t {
    int x;
    int y;
    int t;
};

struct enemy_t {
    int _x;
    int _y;
    int _t;
    int _score;
    int _state;
    int _id;

    void init(int x, int y, int t, int id) {
        _x = x;
        _y = y;
        _t = t;
        _state = 0;
        _score = 5;
        _id = id;
    }
};

extern int cha_x;
extern int cha_y;
extern int score;
extern int shield; // signals first turn
extern std::vector<entity_t> entities;
extern std::vector<enemy_t> enemies;

extern int global_uuid_next;

// prototypes
void draw(void);
void draw_explosion(int x, int y, int r);
