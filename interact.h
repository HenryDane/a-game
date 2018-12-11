struct registry_key_t {
    int id;
    int ridx;
    int type; // 0 = entity, 1 = enemy, 2 = character
    int rtype;
/*
    int tex_state;
    int t;
    int sidx;
*/
};

extern std::vector<registry_key_t> registry;

bool init_registry (void);
bool register_object(int id, int type, int ridx, int rtype);
bool get_registry_xy(int id, int &x, int &y);
bool set_registry_xy(int id, int x, int y);
bool update_object (int id, int dx, int dy);

bool damage_object_x(int x, int dmg);
bool damage_object_y(int y, int dmg);
void vertical_beam(int x);
void horizontal_beam(int x);
void place_explosion(int x, int y, int r);
