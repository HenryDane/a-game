#define PLUS_ENTITY_TEX 0
#define COIN_ENTITY_TEX 1
#define BOMB_ENTITY_TEX 2
#define WALL_ENTITY_TEX 3
#define DOOR_ENTITY_TEX 4
#define TIME_ENTITY_TEX 5

#define ENEMY_TEX 6
#define VLASER_TEX 7
#define HLASER_TEX 8
#define BOSS_TEX 9
#define DEAD_TEX 15

#define EXPLOSION_TEX 10
#define VLASER_BEAM_TEX 11
#define HLASER_BEAM_TEX 12
#define VLASER_PRE_BEAM_TEX 17
#define HLASER_PRE_BEAM_TEX 16

#define CHAR_TEX 13

#define FLOOR_TEX 14

#define NUM_TEX_FILES 18

#include <SFML/Graphics.hpp>
#include <vector>

bool init_display(void);
bool jumpwr_xyt(int x, int y, int t);
bool jumptex_xyt(int x, int y, std::string text);
bool jumptex_xyt(int x, int y, std::string text, sf::Color c);
bool sf_line(float x1, float y1, float x2, float y2);
bool clear_display(void);
char get_dkey(void);

extern std::vector<sf::Texture> textures;
extern sf::Font font;
