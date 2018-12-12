bool respawn_entity(int idx);

bool player_set_safe(void);
bool make_entity_at(int x, int y, int t);
void tick_enemy(enemy_t &en, std::vector<entity_t> &e, int cx, int cy);

void do_gen_next_level(void);

void do_death_screen(void);
void do_win_screen(void);
void do_level_screen(int lvl);

void do_new_game();
void load_game(int slot);
void save_game(int slot);
void goto_loaded_game();
void select_load_level(int lvl);
