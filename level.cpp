#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "patch.h"
#include "game.h"
#include "interact.h"

struct tile_t {
	bool visited;

	bool n;
	bool s;
	bool e;
	bool w;

	bool solid;
};

struct point_t {
	int x;
	int y;
};

void set_tile(tile_t & t, bool v, bool N, bool S, bool E, bool W) {
	t.visited = v;
	t.n = N;
	t.s = S;
	t.e = E;
	t.w = W;

	t.solid = false;
}

/*
void print_maze(tile_t * g) {
	//std::cout << "maze:" << std::endl;
	for (int y = 0; y < h; y++){
		std::string ln1;
		std::string ln2;
		std::string ln3;
		for (int x = 0; x < w; x++){
			ln1 += "O" + patch::to_string((g[y * w + x].n) ? " " : "#") + "O";
			ln2 += patch::to_string((g[y * w + x].e) ? " " : "#") + patch::to_string((g[y * w + x].visited) ? " " : "#") + patch::to_string((g[y * w + x].w) ? " " : "#");
			ln3 += "O" + patch::to_string((g[y * w + x].s) ? " " : "#") + "O";
		}
		std::cout << ln1 << std::endl;
		std::cout << ln2 << std::endl;
		std::cout << ln3 << std::endl;
	}
	//std::cout << "done." << std::endl;
}
*/

int gen_maze(int w, int h){

    int stack_size = w * h;

	// create and init grid
	tile_t * grid = new tile_t[w * h];
	for (int i = 0; i < w * h; i++)
		set_tile(grid[i], false, false, false, false, false);

	// create and init stack
	int stk_addr = 0;
	point_t * stk = new point_t[stack_size];
	for (int i = 0; i < stack_size; i++){
		stk[i].x = 0;
		stk[i].y = 0;
	}

	// main algorithm
	int cx = rand() % w;
	int cy = rand() % h;

	// push current onto stack
	//stk_addr++; // leave {-1, -1} as first element
	stk[stk_addr++] = {cx, cy};

	int cycle = 1;
	while (true) {
		//std::cout << "cycle " << cycle << "\n";
		// set current cell as visited
		//grid[cy * w + cx].visited = true;

		// get unvisited neighbors
		int n[4] = {0, 0, 0, 0};
		if (cx + 1 < w) {
			n[0] = !grid[cy * w + (cx + 1)].visited; // east
		}
		if (cx - 1 >= 0) {
			n[1] = !grid[cy * w + (cx - 1)].visited; // west
		}
		if (cy + 1 < h) {
			n[2] = !grid[(cy + 1) * w + cx].visited; // south
		}
		if (cy - 1 >= 0) {
			n[3] = !grid[(cy - 1) * w + cx].visited; // north
		}

		if (n[0] + n[1] + n[2] + n[3] > 0) { // if 1 or more unvisited neighbors
			// choose random neighbor
			int idx = -1;
			while (idx < 0 || n[idx] == 0) {
				idx = rand() % 4;
			}

			// grid[cy * w + cx].visited = true;

			// connect current to neighbors and set neighbor as current cell
			switch (idx){
			case 0:
				grid[(cx + 1) + cy * w].e = true;
				grid[cx + cy * w].w = true;
				cx++;
				break;
			case 1:
				grid[(cx - 1) + cy * w].w = true;
				grid[cx + cy * w].e = true;
				cx--;
				break;
			case 2:
				grid[cx + (cy + 1) * w].n = true;
				grid[cx + cy * w].s = true;
				cy++;
				break;
			case 3:
				grid[cx + (cy - 1) * w].s = true;
				grid[cx + cy * w].n = true;
				cy--;
				break;
			}

			grid[cy * w + cx].visited = true;

			// push current cell to stack
			if (stk_addr + 1 < stack_size) {
				stk[stk_addr++] = {cx, cy};
			} else {
				std::cout << "failed to push to stack!" << std::endl;
				std::cout << "    " << stk_addr + 1 << " >= " << stack_size << std::endl;
			}

		} else { // else
			// pop stack into current cell
			point_t p = stk[stk_addr--];
			//std::cout << "reading from " << stk_addr + 1 << " found " << p.x << " , " << p.y << std::endl;
			cx = p.x;
			cy = p.y;
		}

		// if all cells visited exit
		bool flag = true;
		for (int i = 0; i < w * h; i++){
			if (!grid[i].visited) {
				flag = false;
				break;
			}
		}

		if (flag) {
			break;
		}

		/*std::cout << "=========== cycle " << cycle << " ==============" << std::endl;
		print_maze(grid);
		std::cout << std::endl;*/

		cycle++;
	}

	for (int y = 0; y < h; y++){
		for (int x = 0; x < w; x++){
			/*ln1 += "O" + patch::to_string((g[y * w + x].n) ? " " : "#") + "O";
			ln2 += patch::to_string((g[y * w + x].e) ? " " : "#") + patch::to_string((g[y * w + x].visited) ? " " : "#") + patch::to_string((g[y * w + x].w) ? " " : "#");
			ln3 += "O" + patch::to_string((g[y * w + x].s) ? " " : "#") + "O";*/
			// corners
			make_entity_at(3 * x, 3 * y, 4);
			make_entity_at(3 * x + 2, 3 * y, 4);
			make_entity_at(3 * x + 2, 3 * y + 2, 4);
			make_entity_at(3 * x, 3 * y + 2, 4);

			if (!grid[y * w + x].n) make_entity_at(3 * x + 1, 3 * y, 4);
			if (!grid[y * w + x].w) make_entity_at(3 * x + 2, 3 * y + 1, 4);
			if (!grid[y * w + x].s) make_entity_at(3 * x + 1, 3 * y + 2, 4);
			if (!grid[y * w + x].e) make_entity_at(3 * x, 3 * y + 1, 4);

			if (!grid[y * w + x].visited) make_entity_at(3 * x + 1, 3 * y + 1, 4);
		}
	}

	// cleanup memory
	delete[] grid;
	delete[] stk;

	// leave
	return true;
}


void place_doors(void){
    make_entity_at(0, 0, 5);
    make_entity_at(S_WIDTH, 0, 5);
    make_entity_at(S_WIDTH, S_HEIGHT, 5);
    make_entity_at(0, S_HEIGHT, 5);
}

void place_ring_xyt(int x, int y, int t){
    make_entity_at(x - 1, y - 1, t);
    make_entity_at(x, y - 1, t);
    make_entity_at(x + 1, y - 1, t);
    make_entity_at(x - 1, y + 1, t);
    make_entity_at(x, y + 1, t);
    make_entity_at(x + 1, y + 1, t);
    make_entity_at(x - 1, y, t);
    make_entity_at(x + 1, y, t);
}

bool generate_empty(void){
    regen_on = true;
    timer_on = -1;

    entities.clear();
    enemies.clear();

    return true;
}

bool generate_tutorial(void){
    regen_on = false;
    timer_on = -1;

    entities.clear();
    enemies.clear();

    cha_x = 2; cha_y = 12;

    make_entity_at(30, 12 + 1, 4 );
    make_entity_at(30, 12 - 1, 4 );

    make_entity_at(50, 12, 1);

    make_entity_at(55, 12, 3);
    make_entity_at(56, 12, 3);

    make_entity_at(60, 12, 2);

    make_entity_at(69, 12, 4);
    enemy_t enemy;
    enemy.init(70, 12, -1, global_uuid_next++);
    enemies.push_back(enemy);
    register_object(enemy._id, 1, 0, -1);

    make_entity_at(80, 12, 5);

    return true;
}

bool generate_maze(void ) {
    regen_on = true;

    entities.clear();
    enemies.clear();

    timer_on = -10;

    gen_maze((S_WIDTH + 3) / 3, S_HEIGHT / 3);

    make_entity_at(1, 1, 5);
    make_entity_at(S_WIDTH - 1, 1, 5);
    make_entity_at(S_WIDTH - 1, S_HEIGHT - 2, 5);
    make_entity_at(1, S_HEIGHT - 2, 5);
}

bool generate_lasers(void) {
    regen_on = true;

    entities.clear();
    enemies.clear();

    for (int i = 0; i < 60; i++){
        make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 4);
    }

    for (int i = 0; i < 20; i++){
        enemy_t e;
        e.init(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 2 + 2, global_uuid_next++);
        enemies.push_back(e);
        e._score = 100000;
        register_object(e._id, 1 /*entity*/, i, 0);
    }

    place_doors();

    return true;
}

bool generate_pacman(void) {
    entities.clear();
    enemies.clear();
    regen_on = false;
    timer_on = -10;

    entity_spawn_lock = true;

    for (int i = 0; i < S_WIDTH + 1; i++){
        for (int j = 0; j < S_HEIGHT + 1; j++){
            if ((i == 0 && j == 0) ||
                (i == S_WIDTH && j == 0 ) ||
                (i == S_WIDTH && j == S_HEIGHT) ||
                (i == 0 && j == S_HEIGHT)) continue;

            int k = rand() % 10;
            if (k == 0 || k == 1){
                make_entity_at(i, j, 4);
            } else if (k == 2 || k == 3 || k == 4) {
                make_entity_at(i, j, 2);
            } else {
                make_entity_at(i, j, (rand() % 3 == 0) ? 3 : 1);
            }
        }
    }

    place_doors();

    entity_spawn_lock = false;

    return true;
}

bool generate_pacman_time(void) {
    entities.clear();
    enemies.clear();
    regen_on = false;
    timer_on = 10;

    entity_spawn_lock = true;

    for (int i = 0; i < S_WIDTH + 1; i++){
        for (int j = 0; j < S_HEIGHT + 1; j++){
            if ((i == 0 && j == 0) ||
                (i == S_WIDTH && j == 0 ) ||
                (i == S_WIDTH && j == S_HEIGHT) ||
                (i == 0 && j == S_HEIGHT)) continue;

            int k = rand() % 10;
            if (k == 0 || k == 1){
                make_entity_at(i, j, 4);
            } else if (k == 2 || k == 3 || k == 4) {
                make_entity_at(i, j, 2);
            } else if (k == 5) {
                make_entity_at(i, j, (rand () % 2 == 0) ? 6 : ((rand() % 3 == 0) ? 3 : 1));
            } else {
                make_entity_at(i, j, (rand() % 3 == 0) ? 3 : 1);
            }
        }
    }

    place_doors();

    entity_spawn_lock = false;

    return true;
}

bool generate_speedrun(void) {
    regen_on = true;
    timer_on = 5; // if no regen, use 18

	cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    entities.clear();
    enemies.clear();

	make_entity_at(cha_x - 1, cha_y - 1, 6);
    make_entity_at(cha_x, cha_y - 1, 6);
    make_entity_at(cha_x + 1, cha_y - 1, 6);
    make_entity_at(cha_x - 1, cha_y + 1, 6);
    make_entity_at(cha_x, cha_y + 1, 6);
    make_entity_at(cha_x + 1, cha_y + 1, 6);
    make_entity_at(cha_x - 1, cha_y, 6);
    make_entity_at(cha_x + 1, cha_y, 6);

	for (int i = 0; i < S_WIDTH + 1; i++){
        for (int j = 0; j < S_HEIGHT + 1; j++){
            if ((i == 0 && j == 0) ||
                (i == S_WIDTH && j == 0 ) ||
                (i == S_WIDTH && j == S_HEIGHT) ||
                (i == 0 && j == S_HEIGHT)) continue;

            if (i == S_WIDTH / 2 && j == S_HEIGHT / 2) continue;

            if (i % 2 == 0 && j % 2 == 0) {
                make_entity_at(i, j, 4);
            }
        }
    }

	make_entity_at(0, 0, 5);
    make_entity_at(S_WIDTH, 0, 5);
    make_entity_at(S_WIDTH, S_HEIGHT, 5);
    make_entity_at(0, S_HEIGHT, 5);

	return true;
}

bool generate_gridworld(void) {
    regen_on = true;
    timer_on = 10;

	cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    entities.clear();
    enemies.clear();

	for (int i = 0; i < S_WIDTH; i++){
        for (int j = 0; j < S_HEIGHT; j++){
            if (rand() % 4 == 0){
                make_entity_at(i, j, 4);
            } else {
                if (rand() % 5 == 0) {
                    make_entity_at(i, j, (rand() % 3 == 0) ? 6 : 1);
                }
	        }
        }
    }

	make_entity_at(0, 0, 5);
    make_entity_at(S_WIDTH, 0, 5);
    make_entity_at(S_WIDTH, S_HEIGHT, 5);
    make_entity_at(0, S_HEIGHT, 5);

	return true;
}

bool generate_impossible(){
    regen_on = true;
    respawn_bomb_on = false;
    timer_on = -10; // if no regen, use 18
    cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    entities.clear();
    enemies.clear();

    for (int i = 0; i < S_WIDTH + 1; i++){
        for (int j = 0; j < S_HEIGHT + 1; j++){
            if ((i == 0 && j == 0) ||
                (i == S_WIDTH && j == 0 ) ||
                (i == S_WIDTH && j == S_HEIGHT) ||
                (i == 0 && j == S_HEIGHT)) continue;

            if (abs(i- cha_x) <= 3 && abs(j - cha_y) <= 3) continue;

            if (i % 2 == 0 && j % 2 == 0) {
                make_entity_at(i, j, 2);
            }
        }
    }

    place_ring_xyt(cha_x, cha_y, 3); // so that coins are on top

    place_doors();

    return true;
}

bool generate_terrain(void){
    regen_on = true;
    timer_on = -10;
    cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    entities.clear();
    enemies.clear();

    for (int i = 0; i < 60; i++){
        make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 4);
    }

    for (int i = 0; i < 10; i++){
        enemy_t e;
        e.init(rand() % S_WIDTH, rand() % S_HEIGHT, 0, global_uuid_next++);
        enemies.push_back(e);
        register_object(e._id, 1 /*entity*/, i, 0);
    }

    for (int i = 0; i < 20; i++){
        int x = rand() % S_WIDTH;
        int y = rand() % S_HEIGHT;
        switch(rand() % 4){
        case 0: // L block
            make_entity_at(x, y, 4);
            make_entity_at(x + 1, y, 4);
            make_entity_at(x + 1, y + 1, 4);
            make_entity_at(x + 1, y + 2, 4);
            break;
        case 1: // make u block
            make_entity_at(x, y, 4);
            make_entity_at(x + 1, y, 4);
            make_entity_at(x + 2, y, 4);
            make_entity_at(x + 2, y + 1, 4);
            make_entity_at(x + 2, y + 2, 4);
            make_entity_at(x, y + 1, 4);
            make_entity_at(x, y + 2, 4);
            break;
        case 2: // make ii block
            make_entity_at(x, y + 1, 4);
            make_entity_at(x, y + 2, 4);
            make_entity_at(x + 2, y, 4);
            make_entity_at(x + 2, y + 1, 4);
            break;
        case 3:
            make_entity_at(x, y, 4);
            make_entity_at(x + 2, y + 2, 4);
            make_entity_at(x, y + 2, 4);
            make_entity_at(x + 2, y, 4);
            break;
        default:
            break;
        }
    }

    place_doors();

    return true;
}

bool generate_dense_terrain( void ){
    regen_on = true;
    timer_on = -10;
    cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    entities.clear();
    enemies.clear();

    entity_overlap_check_on = true;

    int ent_spawn = 0;
    while (ent_spawn < 100){
        ent_spawn += (make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 4) ? 1 : 0);
    }

    for (int i = 0; i < 10; i++){
        enemy_t e;
        e.init(rand() % S_WIDTH, rand() % S_HEIGHT, 0, global_uuid_next++);
        enemies.push_back(e);
        register_object(e._id, 1 /*entity*/, i, 0);
    }

    for (int i = 0; i < 20; i++){
        enemy_t e;
        e.init(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 2 + 2, global_uuid_next++);
        enemies.push_back(e);
        e._score = 100000;
        register_object(e._id, 1 /*entity*/, i + 10, 0);
    }

    for (int i = 0; i < 50; i++){
        int x = rand() % S_WIDTH;
        int y = rand() % S_HEIGHT;
        switch(rand() % 4){
        case 0: // L block
            make_entity_at(x, y, 4);
            make_entity_at(x + 1, y, 4);
            make_entity_at(x + 1, y + 1, 4);
            make_entity_at(x + 1, y + 2, 4);
            break;
        case 1: // make u block
            make_entity_at(x, y, 4);
            make_entity_at(x + 1, y, 4);
            make_entity_at(x + 2, y, 4);
            make_entity_at(x + 2, y + 1, 4);
            make_entity_at(x + 2, y + 2, 4);
            make_entity_at(x, y + 1, 4);
            make_entity_at(x, y + 2, 4);
            break;
        case 2: // make ii block
            make_entity_at(x, y + 1, 4);
            make_entity_at(x, y + 2, 4);
            make_entity_at(x + 2, y, 4);
            make_entity_at(x + 2, y + 1, 4);
            break;
        case 3:
            make_entity_at(x, y, 4);
            make_entity_at(x + 2, y + 2, 4);
            make_entity_at(x, y + 2, 4);
            make_entity_at(x + 2, y, 4);
            break;
        default:
            break;
        }
    }

    entity_overlap_check_on = false;

    make_entity_at(0, 0, 5);
    make_entity_at(S_WIDTH, 0, 5);
    make_entity_at(S_WIDTH, S_HEIGHT, 5);
    make_entity_at(0, S_HEIGHT, 5);

    return true;
}

bool generate_safe_run(void) {
    generate_terrain();

    regen_on = true;
    timer_on = 35;

    for (int i = 0; i < 60; i++){
        make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, 6);
    }

    return true;
}

bool generate_boss(void){
    cha_x = S_WIDTH / 2;
    cha_y = S_HEIGHT / 2;
    entities.clear();
    enemies.clear();

    regen_on = false;
    timer_on = 50;

    for (int i = 0; i < 50; i++){
        make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, 3);
        make_entity_at(rand() % S_WIDTH, rand() % S_HEIGHT, 2);
    }

    make_entity_at(cha_x - 1, cha_y - 1, 3);
    make_entity_at(cha_x, cha_y - 1, 3);
    make_entity_at(cha_x + 1, cha_y - 1, 3);
    make_entity_at(cha_x - 1, cha_y + 1, 3);
    make_entity_at(cha_x, cha_y + 1, 3);
    make_entity_at(cha_x + 1, cha_y + 1, 3);
    make_entity_at(cha_x - 1, cha_y, 3);
    make_entity_at(cha_x + 1, cha_y, 3);

    // ADD BOSS ENEMY
    enemy_t boss;
    boss.init(rand() % S_WIDTH, rand() % S_HEIGHT, 4, global_uuid_next++);
    boss._score = 50;
    enemies.push_back(boss);
    register_object(boss._id, 1 /*entity*/, 0, 4);

    for(int i = 0; i < 10; i++){
        enemy_t e;
        e.init(rand() % S_WIDTH, rand() % S_HEIGHT, rand() % 2 + 2, global_uuid_next++);
        e._score = 100000;
        enemies.push_back(e);
        register_object(e._id, 1, i + 1, e._t);
    }

    return true;
}
