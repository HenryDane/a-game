bool respawn_entity(int idx);

bool player_set_safe(void);
bool make_entity_at(int x, int y, int t);
void tick_enemy(enemy_t &en, std::vector<entity_t> &e, int cx, int cy);

void do_gen_next_level(void);

void do_death_screen(void);
void do_win_screen(void);
void do_level_screen(int lvl);
