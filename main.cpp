#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include "main.h"
#include "display.h"
#include "game.h"
#include "interact.h"
#include "patch.h"
#include "draw.h"
#include "audio.h"
#include "level.h"

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
int turns = 0;

// flags
bool regen_on = true; // do entities regen?
int timer_on = -1; // if >= 0 timer is on
bool dots_on = false; // flag for dots display
bool entity_spawn_lock = false; // flag for deleting everything nearby
bool entity_overlap_check_on = false; // prevent overlap spawn
bool respawn_bomb_on = true;
bool debug = false;
bool cheats = false;
bool game_session_valid = false;

// menu shenanigans
int gen_idx = 0;
int gen_idx_ulim = 7; // exclusive

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

    std::cout << window.getSize().x << "," << window.getSize().y << std::endl;

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

    decrease_volume(100);

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
                } else if (event.key.code == sf::Keyboard::LBracket) { // '['
                    decrease_volume(20);
                } else if (event.key.code == sf::Keyboard::RBracket) { // '['
                    increase_volume(20);
                } else if (event.key.code == sf::Keyboard::BackSlash) {
                    skip_current_song();
                }

                if (cheats) {
                    if (event.key.code == sf::Keyboard::Add) {
                        state++;
                    } else if (event.key.code == sf::Keyboard::Subtract) {
                        state--;
                    } else if (event.key.code == sf::Keyboard::Enter) {
                        std::cout << "State: " << state << std::endl;
                    } else if (event.key.code == sf::Keyboard::Tab) {
                        //std::cout << music.getPlayingOffset().asSeconds() << " | " << music.getDuration().asSeconds() << std::endl;
                        score += 1000;
                    } else if (event.key.code == sf::Keyboard::Tilde) { // conflict with menu key`
                        skip_seconds(30);
                    }
                }

                if (state >= 10) {
                    if (event.key.code == sf::Keyboard::Numpad8 ||
                        event.key.code == sf::Keyboard::Up ||
                        event.key.code == sf::Keyboard::W) {
                        gen_idx--;
                    } else if (event.key.code == sf::Keyboard::Numpad2 ||
                               event.key.code == sf::Keyboard::Down ||
                               event.key.code == sf::Keyboard::S) {
                        gen_idx++;
                    }
                }

                switch (state) {
                case 0: // title screen
                    if (event.key.code == sf::Keyboard::Space) {
                        state = 10; // tutorial pre-screen
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
                    game_session_valid = false;
                    if (event.key.code == sf::Keyboard::Space) {
                        //window.close();
                        //return 0;
                        state = 10;
                    }
                    break;
                case 10: // main menu
                    if (event.key.code == sf::Keyboard::Right ||
                        event.key.code == sf::Keyboard::Numpad6 ||
                        event.key.code == sf::Keyboard::D) {
                        switch (gen_idx) {
                            case 0: state = 11; break;
                            case 1: state = 12; break;
                            case 2: state = 13; break;
                            case 3:
                                state = 14;
                                gen_idx = level;
                                break;
                            case 4: state = 15; break;
                            case 6: state = 16; break;
                            case 5:
                                if (game_session_valid) state = 1;
                                break;
                            default: break;
                        }
                    } else if (event.key.code == sf::Keyboard::H) {
                        state = 20;
                    }
                    break;
                case 11: // new game
                    // any key to start
                    game_session_valid = true;
                    do_new_game();
                    break;
                case 12: // load game
                    if (event.key.code == sf::Keyboard::Right ||
                        event.key.code == sf::Keyboard::Numpad6 ||
                        event.key.code == sf::Keyboard::D) {
                        load_game(gen_idx);
                    } else if (event.key.code == sf::Keyboard::Left ||
                        event.key.code == sf::Keyboard::Numpad4 ||
                        event.key.code == sf::Keyboard::A) {
                        state = 10;
                    }
                    break;
                case 13: // save game
                    if (event.key.code == sf::Keyboard::Right ||
                        event.key.code == sf::Keyboard::Numpad6 ||
                        event.key.code == sf::Keyboard::D) {
                        save_game(gen_idx);
                        state = 18; // game saved
                    } else if (event.key.code == sf::Keyboard::Left ||
                        event.key.code == sf::Keyboard::Numpad4 ||
                        event.key.code == sf::Keyboard::A) {
                        state = 10;
                    }
                    break;
                case 14: // level select
                    if (event.key.code == sf::Keyboard::Right ||
                        event.key.code == sf::Keyboard::Numpad6 ||
                        event.key.code == sf::Keyboard::D) {
                        select_load_level(gen_idx);
                        state = 2; // press any key to begin selected level
                    } else if (event.key.code == sf::Keyboard::Left ||
                        event.key.code == sf::Keyboard::Numpad4 ||
                        event.key.code == sf::Keyboard::A) {
                        gen_idx = 3;
                        state = 10;
                    }
                    break;
                case 15: // credits
                    state = 10; // go to main menu
                    break;
                case 16: // options
                    // TODO (add main menu option)
                    // TODO (add resume game option (only if there is a valid game ready))
                    if (event.key.code == sf::Keyboard::Right ||
                        event.key.code == sf::Keyboard::Numpad6 ||
                        event.key.code == sf::Keyboard::D) {
                        switch (gen_idx) {
                        case 0:
                            // vol adjust
                            break;
                        case 1:
                            // debug
                            debug = !debug;
                            break;
                        case 2:
                            // cheats
                            cheats = !cheats;
                            break;
                        case 3:
                            // reload audio
                            update_sound();
                            break;
                        case 4:
                            // reload gfx
                            init_display();
                            break;
                        case 5:
                            if (debug) std::cout << "Save erasure is not implemented" << std::endl;
                            // erase all saves
                            break;
                        }
                    } else if (event.key.code == sf::Keyboard::Left ||
                        event.key.code == sf::Keyboard::Numpad4 ||
                        event.key.code == sf::Keyboard::A) {
                        state = 10;
                    }
                    break;
                case 17: // press any key to begin loaded game
                    goto_loaded_game();
                    break;
                case 18: // game saved
                    if (event.key.code == sf::Keyboard::Right ||
                        event.key.code == sf::Keyboard::Numpad6 ||
                        event.key.code == sf::Keyboard::D) {
                        state = 13;
                    } else if (event.key.code == sf::Keyboard::Left ||
                        event.key.code == sf::Keyboard::Numpad4 ||
                        event.key.code == sf::Keyboard::A) {
                        state = 10;
                    }
                    break;
                case 20:
                    if (event.key.code == sf::Keyboard::Space) {
                        state = 10;
                    }
                    break;
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
        case 10: // main menu
            if (gen_idx < 0) gen_idx = 6;
            if (gen_idx > 6) gen_idx = 0;
            draw_main_menu();
            break;
        case 11: // new game
            draw_new_game();
            break;
        case 12: // load game
            if (gen_idx < 0) gen_idx = 5;
            if (gen_idx > 5) gen_idx = 0;
            draw_load_game();
            break;
        case 13: // save game
            if (gen_idx < 0) gen_idx = 5;
            if (gen_idx > 5) gen_idx = 0;
            draw_save_game();
            break;
        case 14: // level select
            if (gen_idx < 0) gen_idx = 19;
            if (gen_idx > 19) gen_idx = 0;
            draw_level_select();
            break;
        case 15: // credits
            draw_credits();
            break;
        case 16: // options
            if (gen_idx < 1) gen_idx = 5;
            if (gen_idx > 5) gen_idx = 1;
            draw_options();
            break;
        case 17: // press any key to begin loaded game
            draw_new_game();
            break;
        case 18:
            // draw saved game notificatipon here
            draw_game_saved();
            break;
        case 20:
            draw_help();
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
    case 'z': // main menu
    case 'Z':
        state = 10;
        return 0;
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
            shield += 10;
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
    if (timer_on == 0 && level == 19){
        //do_win_screen();
        make_entity_at(S_WIDTH / 2, S_HEIGHT / 2, 100);
    } else if (timer_on == 0 && level != 19) {
        score -= 1000;
    }

    // game over check
    if (score < -20) {
        do_death_screen();
        return 1;
    }

    // handle particles
    for (unsigned int i = 0; i < particles.size(); i++){
        particles[i].ttl --;

        if (particles[i].ttl < 0) {
            particles.erase(particles.begin() + i);
            //if (i == 0) i = 0;
            if (i >= 0) i--;
        }
    }

    turns++;

    return 0;
}
