#include "main.h"
#include "patch.h"
#include "display.h"
#include "audio.h"

void draw_tutorial_level_text(void){
    jumptex_xyt(0, 2, "Press 't' to skip this tutorial");

    if (cha_x > 0) {
        jumptex_xyt(0, 9, "Use WASD to move");
    }
    if (cha_x > 25) {
        jumptex_xyt(20, 10, "Avoid walls '#' ");
    }
    if (cha_x > 45) {
        jumptex_xyt(40, 11, "Collect '+' tokens for +1 score");
        jumptex_xyt(45, 12, "Collect '$' tokens for +5 score");
    }
    if (cha_x > 55) {
        jumptex_xyt(50, 16, "Avoid '!' bombs. They have a radius of 2.");
        jumptex_xyt(55, 17, "They do damage of -10 to your score.");
    }
    if (cha_x > 65) {
        jumptex_xyt(60, 20, "Enemies 'O' subtract 200 from your score.");
    }

    if (cha_x > 75) {
        jumptex_xyt(0, 4, "Press 'g' to turn on and off dots in the background. This does not effect gameplay.");
        jumptex_xyt(0, 5, "Press 'q' to activate your shield and protect yourself from bombs (only works if your score is above ten).");
        jumptex_xyt(0, 6, "Press 'e' to use a grenade on surrounding enemies (only works if your score is above five).");

        jumptex_xyt(0, 24, "Doors 'D' take you to the next level.");
        jumptex_xyt(0, 25, "Game over occurs when your score is less than -20. Don't let it get that low.");

        jumptex_xyt(0, 27, "Good night, and good luck.");
    }
}

void draw_gfx(void) {
    if (level == 0)  draw_tutorial_level_text();

    for (int i = 0; i < particles.size(); i++){
        int t = EXPLOSION_TEX;
        if (particles[i].type == 1) t = HLASER_BEAM_TEX;
        if (particles[i].type == 2) t = VLASER_BEAM_TEX;

        jumpwr_xyt(particles[i].x, particles[i].y + 2, t);
    }

    // lasers
    for (enemy_t e : enemies){
        //if (e._t == 2) sf_line(e._x * TILE_WIDTH + HALF_TILE_WIDTH, 0 + (2 * TILE_WIDTH), e._x * TILE_WIDTH + HALF_TILE_WIDTH, (S_HEIGHT + 1) * TILE_WIDTH + (2 * TILE_WIDTH));
        //if (e._t == 3) sf_line(0, e._y * TILE_WIDTH + HALF_TILE_WIDTH  + (2 * TILE_WIDTH), (S_WIDTH + 1) * TILE_WIDTH, e._y * TILE_WIDTH + HALF_TILE_WIDTH + (2 * TILE_WIDTH));
    }

    // entities
    for (entity_t e : entities){
        if (e.t < 0) continue;

        switch (e.t){
        case 1:
        case 0:
            jumpwr_xyt(e.x, e.y + 2, PLUS_ENTITY_TEX); break;
        case 2:
            jumpwr_xyt(e.x, e.y + 2, BOMB_ENTITY_TEX); break;
        case 3:
            jumpwr_xyt(e.x, e.y + 2, COIN_ENTITY_TEX); break;
        case 4:
            jumpwr_xyt(e.x, e.y + 2, WALL_ENTITY_TEX); break;
        case 100:
        case 5:
            jumpwr_xyt(e.x, e.y + 2, DOOR_ENTITY_TEX); break;
        case 6:
            jumpwr_xyt(e.x, e.y + 2, TIME_ENTITY_TEX); break;
        default:
            break;
        }
    }

    // enemies
    for (enemy_t e : enemies){
        if (e._state < 0) {
            jumpwr_xyt(e._x, e._y + 2, DEAD_TEX);
            continue;
        }

        if (e._t == 2 || e._t == 3) {
            jumpwr_xyt(e._x, e._y + 2, (e._t == 2) ? VLASER_TEX : HLASER_TEX);
            if (e._state < 0){
                jumptex_xyt(e._x, e._y + 2, "-", sf::Color(255, 0, 0));
            } else if (e._state >= 10){
                jumptex_xyt(e._x, e._y + 2, "*", sf::Color(255, 0, 0));
            } else {
                jumptex_xyt(e._x, e._y + 2, patch::to_string(e._state), sf::Color(255, 0, 0));
            }
        } else if (e._t == 4) {
            jumpwr_xyt(e._x, e._y + 2, BOSS_TEX);
        } else {
            jumpwr_xyt(e._x, e._y + 2, ENEMY_TEX);
        }
    }

    // score and other indicators
    jumptex_xyt(0, 0, "SCORE: " + patch::to_string(score) + " SHIELD: " + patch::to_string(((shield <= 0) ? 0 : shield)) + " LEVEL: " + patch::to_string(level) + " TIMER: " + ((timer_on < 0) ? "infinite" : patch::to_string(timer_on)));
    jumptex_xyt(0, 1, "S:" + patch::to_string(state) + " P:" + patch::to_string(particles.size()) + " E:" + patch::to_string(entities.size()) + " M:" + patch::to_string(enemies.size()) + " A:" + patch::to_string(current_music_state));

    jumpwr_xyt(cha_x, cha_y + 2, CHAR_TEX);
}

void draw_title_screen(){
    sf::Text text;
    text.setString("a_game");
    text.setPosition(10,10);
    text.setCharacterSize(50);
    text.setColor(sf::Color(255, 255, 255)); // todo animate colors
    text.setFont(font);

    renderTexture.draw(text);
}

void draw_level_screen(int lvl){
    sf::Text text;
    text.setString("Level " + patch::to_string(lvl));
    text.setPosition(10,10);
    text.setCharacterSize(50);
    text.setColor(sf::Color(255, 255, 255)); // todo animate colors
    text.setFont(font);

    renderTexture.draw(text);
}

void draw_death_screen(void){
    sf::Text text;
    text.setString("Game Over");
    text.setPosition(10,10);
    text.setCharacterSize(50);
    text.setColor(sf::Color(255, 255, 255)); // todo animate colors
    text.setFont(font);

    renderTexture.draw(text);
}

void draw_win_screen(void){
    sf::Text text;
    text.setString("You Win");
    text.setPosition(10,10);
    text.setCharacterSize(50);
    text.setColor(sf::Color(255, 255, 255)); // todo animate colors
    text.setFont(font);

    renderTexture.draw(text);
}
