bool respawn_entity(int idx);

bool generate_pacman(void);
bool generate_terrain(void);
bool generate_lasers(void);
bool generate_safe_run(void);
bool generate_boss(void);

bool player_set_safe(void);
bool make_entity_at(int x, int y, int t);
void tick_enemy(enemy_t &en, std::vector<entity_t> &e, int cx, int cy);

void do_gen_next_level(void);
