struct registry_key_t {
    int id;
    int ridx;
    int type; // 0 = entity, 1 = enemy, 2 = character
    int rtype;
};

extern std::vector<registry_key_t> registry;

bool init_registry (void);
bool register_object(int id, int type, int ridx, int rtype);
bool get_registry_xy(int id, int &x, int &y);
bool set_registry_xy(int id, int x, int y);
bool update_object (int id, int dx, int dy);
