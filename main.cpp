#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include "main.h"
#include "display.h"
#include "console.h"
#include "game.h"
#include "interact.h"
#include "patch.h"
#include "tex_print.h"
#include "draw.h"
#include "audio.h"

// globals lol
int cha_x = S_WIDTH / 2;
int cha_y = S_HEIGHT / 2;
int score = 5;
int global_score = 0;
int level = 0;
int shield = -1002; // signals first turn
std::vector<entity_t> entities;
std::vector<enemy_t> enemies;
std::vector<particle_t> particles;
int global_uuid_next = 0;

// flags
bool regen_on = true; // do entities regen?
int timer_on = -1; // if >= 0 timer is on
bool dots_on = false; // flag for dots display
bool entity_spawn_lock = false; // flag for deleting everything nearby
bool entity_overlap_check_on = false; // prevent overlap spawn
bool respawn_bomb_on = true;

// GAME STATE
int state = 0;

// newline
char __NL = 10;

//gfx shenanigans
sf::RenderTexture renderTexture;

int main(){
    // configure random
    srand( time( NULL) );

    // create window
    sf::RenderWindow window(sf::VideoMode((S_WIDTH + 1) * TILE_WIDTH, (S_HEIGHT + 3) * TILE_WIDTH), "SFML works!");

    // generate render texture
    if (!renderTexture.create((S_WIDTH + 1) * TILE_WIDTH, (S_HEIGHT + 3) * TILE_WIDTH)) return 1000;

    // initalize textures
    init_display();

    // allocate memory in vectors to avoid issues
    init_registry();
    entities.reserve(100);
    enemies.reserve(30);
    particles.reserve(100);

    register_object(global_uuid_next++, 2, 0, 0); // register player

    generate_tutorial(); // set up map

    player_set_safe(); // go to safe location

    update_sound();

    //decrease_volume(100);

    while (window.isOpen()) {
        //clear the screen
		window.clear();
        renderTexture.clear();

        check_audio_state();

		//process events
		sf::Event event;
		while (window.pollEvent(event)) {
			//close when the window is closed
			if (event.type == sf::Event::Closed) {
				window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                // exit on esc
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                    return 0;
                } else if (event.key.code == sf::Keyboard::Tab) {
                    //std::cout << music.getPlayingOffset().asSeconds() << " | " << music.getDuration().asSeconds() << std::endl;
                    //score += 1000;
                } else if (event.key.code == sf::Keyboard::Tilde) {
                    skip_seconds(30);
                } else if (event.key.code == sf::Keyboard::LBracket) { // '['
                    decrease_volume(20);
                } else if (event.key.code == sf::Keyboard::RBracket) { // '['
                    increase_volume(20);
                } else if (event.key.code == sf::Keyboard::BackSlash) {
                    skip_current_song();
                }

                switch (state) {
                case 0: // title screen
                    if (event.key.code == sf::Keyboard::Space) {
                        state = 3; // tutorial pre-screen
                    }
                    break;
                case 1: // normal gameplay
                    handle_key(event.key.code);
                    break;
                case 2: // level screen
                    if (event.key.code == sf::Keyboard::Space) {
                        state = 1;
                    }
                    break;
                case 3:
                    if (event.key.code == sf::Keyboard::Space) {
                        state = 1; // tutorial screen
                    }
                    break;
                case 5:
                case 4:
                    if (event.key.code == sf::Keyboard::Space) {
                        window.close();
                        return 0;
                    }
                default:
                    std::cout << "bad state!" << std::endl;
                    break;
                }
            }

		}

		// draw
		switch (state){
        case 0:
            draw_title_screen();
            break;
        case 1:
            draw_gfx();
            break;
        case 3:
        case 2:
            // level screen
            draw_level_screen(level);
            break;
        case 4:
            draw_death_screen();
            break;
        case 5:
            draw_win_screen();
            break;
        default:
            break;
		}

        // prep the render texture
        renderTexture.display();

        // get the target texture (where the stuff has been drawn)
        const sf::Texture& texture = renderTexture.getTexture();

        // draw it to the window
        sf::Sprite sprite(texture);
        window.draw(sprite);

        // flip display
		window.display();
	}

    return 0;
}

int handle_key(sf::Keyboard::Key k){
    if (k < 0 || k > 26) return 0; // if not a-z

    char c = ( (int) k ) + 65; // awful hack but ok

    // execute key
    switch (c){
    case 'G':
    case 'g':
        dots_on = !dots_on;
//            draw();
        return 0;
    case 'T':
    case 't': // tutorial skip
        if (level >= 0){ // should be level == 0
            level++;
            timer_on = -1;
            cha_x = S_WIDTH / 2;
            cha_y = S_HEIGHT / 2;

            do_gen_next_level();
            return 0;
        }
        break;
    case 'W':
    case 'w':
        update_object(0, 0, -1);
        break;
    case 'D':
    case 'd':
        update_object(0, 1, 0);
        break;
    case 'S':
    case 's':
        update_object(0, 0, 1);
        break;
    case 'A':
    case 'a':
        update_object(0, -1, 0);
        break;
    case 'Q':
    case 'q':
        if (score > 10 &&
            level != 7 &&
            level != 8 &&
            level != 9) { // activates ten turn shield for a score cost of 5
            shield = 10;
            score -= 5;
        }
        break;
    case 'E':
    case 'e': // uses a grenade with a cost of 5
        if (score > 10 &&
            level != 7 &&
            level != 8 &&
            level != 9) {
            score -= 5;
            place_explosion(cha_x, cha_y, 2);
            //get_key();

            for (unsigned int i = 0; i < enemies.size(); i++){
                if (abs(enemies[i]._x - cha_x) < 3 &&
                    abs(enemies[i]._y - cha_y) < 3){
                    enemies[i]._score -= -10;
                    score += 5;
                }
            }
        }
        break;
    default:
        update_object(0, 0, 0);
        break;
    }

    for (unsigned int i = 0; i < enemies.size(); i++){
        tick_enemy(enemies[i], entities, cha_x, cha_y);
    }

    // first 3 turns are immune to score changes
    if (shield <= -1000){
        shield = 0;
        score = 5;
    }

    // tick shield
    if (shield > 0) shield--; // decrease shield
    if (timer_on > 0) timer_on --;

    // handle timer
    if (timer_on == 0 && level == 15){
        //do_win_screen();
        make_entity_at(S_WIDTH / 2, S_HEIGHT / 2, 100);
    } else if (timer_on == 0 && level != 15) {
        score -= 1000;
    }

    // game over check
    if (score < -20) {
        do_death_screen();
        return 1;
    }

    // handle particles
    for (int i = 0; i < particles.size(); i++){
        particles[i].ttl --;

        if (particles[i].ttl < 0) {
            particles.erase(particles.begin() + i);
            //if (i == 0) i = 0;
            if (i >= 0) i--;
        }
    }

    return 0;
}
