#include <vector>
#include <stdlib.h>

#define S_WIDTH 100
#define S_HEIGHT 25

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

    void init(int x, int y, int t) {
        _x = x;
        _y = y;
        _t = t;
        _state = 0;
        _score = 5;
    }

    void tick(std::vector<entity_t> &e, int cx, int cy){
        if (_t == 0){
            // randomwalks
            _x += rand() % 3 - 1;
            _y += rand() % 3 - 1;
        } else if (_t == 1){ // hunter
            // TODO obj avoid
            _x -= (_x > cx) ? 1 : ((_x == cx) ? 0 : -1 );
            _y -= (_y > cy) ? 1 : ((_y == cy) ? 0 : -1 );
        }

        // boundary check
        if (_x < 0) _x++;
        if (_y < 0) _y++;
        if (_x > S_WIDTH) _x--;
        if (_y > S_HEIGHT) _y--;

        // score check
        if (_score < -20) _state = -1;
    }
};

extern int cha_x;
extern int cha_y;
extern int score;
extern int shield; // signals first turn
extern std::vector<entity_t> entities;
extern std::vector<enemy_t> enemies;
