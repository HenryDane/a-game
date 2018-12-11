#include <SFML/Graphics.hpp>
#include "main.h"
#include "patch.h"
#include "display.h"
#include "audio.h"

/*
sf::Sprite lvlbgs;
sf::RenderTexture lvlbg;

void setup_lvlbg(void){
    sf::Sprite sprite1;
    sprite1.setTexture(textures.at(WALL_ENTITY_TEX));
    sprite1.setScale(TEX_SCALE * 3, TEX_SCALE * 3); // (128 / 4) = 32

    for (int x = 0; x < S_WIDTH; x++){
        for (int y = 0; y < S_HEIGHT; y++){
            sprite1.setPosition(x * TILE_WIDTH, y * TILE_WIDTH);
            lvlbg.draw(sprite1);
        }
    }
}
*/

void draw_tutorial_level_text(void){
    jumptex_xyt(0, 2, "Press 't' to skip this tutorial");

    if (cha_x > 0) {
        jumptex_xyt(0, 9, "Use WASD to move");
    }
    if (cha_x > 25) {
        jumptex_xyt(20, 10, "Avoid walls");
        //jumpwr_xyt(33, 10, WALL_ENTITY_TEX);
    }
    if (cha_x > 45) {
        jumptex_xyt(40, 11, "Collect plus tokens for +1 score");
        //jumpwr_xyt(49, 11, PLUS_ENTITY_TEX);
        jumptex_xyt(45, 12, "Collect coin tokens for +5 score");
        //jumpwr_xyt(54, 12, COIN_ENTITY_TEX);
    }
    if (cha_x > 55) {
        jumptex_xyt(50, 16, "Avoid bombs. They have a radius of 2.");
        //jumpwr_xyt(57, 16, BOMB_ENTITY_TEX);
        jumptex_xyt(55, 17, "They do damage of -10 to your score.");
    }
    if (cha_x > 65) {
        jumptex_xyt(60, 20, "Enemies subtract 200 from your score.");
        //jumpwr_xyt(69, 20, ENEMY_TEX);
    }

    if (cha_x > 75) {
        jumptex_xyt(0, 4, "Press 'g' to turn on and off dots in the background. This does not effect gameplay.");
        jumptex_xyt(0, 5, "Press 'q' to activate your shield and protect yourself from bombs (only works if your score is above ten).");
        jumptex_xyt(0, 6, "Press 'e' to use a grenade on surrounding enemies (only works if your score is above five).");

        jumptex_xyt(0, 24, "Doors take you to the next level.");
        //jumpwr_xyt(7, 24, DOOR_ENTITY_TEX);
        jumptex_xyt(0, 25, "Game over occurs when your score is less than -20. Don't let it get that low.");

        jumptex_xyt(0, 27, "Good night, and good luck.");
    }
}

void draw_gfx(void) {
    if (level == 0)  draw_tutorial_level_text();

/*    const sf::Texture& lvlbgtx = lvlbg.getTexture();
    lvlbgs.setTexture(lvlbgtx);
    renderTexture.draw(lvlbgs); */

    for (int i = 0; i < particles.size(); i++){
        if (particles[i].x < 0 || particles[i].y < 0) continue;

        int t = -1;

        switch(particles[i].type) {
        case 1: t = HLASER_BEAM_TEX; break;
        case 2: t = VLASER_BEAM_TEX; break;
        case 4: t = VLASER_PRE_BEAM_TEX; break;
        case 3: t = HLASER_PRE_BEAM_TEX; break;
        default: t = EXPLOSION_TEX; break;
        }

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
    //text.setPosition(10,10);
    text.setCharacterSize(50);
    text.setColor(sf::Color(255, 255, 255)); // todo animate colors
    text.setFont(font);

    // center text
    sf::FloatRect b = text.getLocalBounds();
    text.setPosition(((S_WIDTH + 1) * 16 - b.width) / 2, ((S_HEIGHT + 3) * 16 - b.height) / 2);

    renderTexture.draw(text);

    text.setString("Press any key to begin");
    text.setCharacterSize(13);
    text.setColor(sf::Color(0, 255, 0));

    // center text
    b = text.getLocalBounds();
    text.setPosition(((S_WIDTH + 1) * 16 - b.width) / 2, ((S_HEIGHT + 3) * 16 - b.height) / 2 + 75);

    renderTexture.draw(text);
}

void draw_level_screen(int lvl){
    sf::Text text;
    text.setString("Level " + patch::to_string(lvl));
    //text.setPosition(10,10);
    text.setCharacterSize(50);
    text.setColor(sf::Color(255, 255, 255)); // todo animate colors
    text.setFont(font);

    // center text
    sf::FloatRect b = text.getLocalBounds();
    text.setPosition(((S_WIDTH + 1) * 16 - b.width) / 2, ((S_HEIGHT + 3) * 16 - b.height) / 2);

    renderTexture.draw(text);

    text.setCharacterSize(13);
    text.setColor(sf::Color(255, 0, 0));

    switch (lvl) {
    case 0:
        text.setString("You can still die on the tutorial!");
        break;
    case 1:
    case 2:
        text.setString("Patience is a virtue");
        break;
    case 3:
    case 12:
        text.setString("Collect time tokens to avoid dying by the clock!");
        break;
    case 4:
    case 5:
        text.setString("Watch out for those lasers!");
        break;
    case 6:
        text.setString("How good is your concentration?");
        break;
    case 7:
        text.setString("This is hard. You'll only have to do it once.");
        break;
    case 8:
    case 9:
        text.setString("I lied. Get baited.");
        break;
    case 10:
        text.setString("Get ready for a bossfight!");
        break;
    case 11:
        text.setString("Suprise. I lied agian. This level will be easy.");
        break;
    case 13:
        text.setString("This is impossible.");
        break;
    case 14:
        text.setString("DEJA VU NEVER BEEN TO THIS PLACE BEFORE !!1!!11one!one!");
        break;
    case 15:
        text.setString("Here's the tutorial agian. Why do you even trust me?");
        break;
    default:
        text.setString("How'd you get to this level?");
        break;
    }

    // center text
    b = text.getLocalBounds();
    text.setPosition(((S_WIDTH + 1) * 16 - b.width) / 2, ((S_HEIGHT + 3) * 16 - b.height) / 2 + 75);
    renderTexture.draw(text);
}

void draw_death_screen(void){
    sf::Text text;
    text.setString("Game Over");
    //text.setPosition(10,10);
    text.setCharacterSize(50);
    text.setColor(sf::Color(255, 0, 0)); // todo animate colors
    text.setFont(font);

    // center text
    sf::FloatRect b = text.getLocalBounds();
    text.setPosition(((S_WIDTH + 1) * 16 - b.width) / 2, ((S_HEIGHT + 3) * 16 - b.height) / 2);

    renderTexture.draw(text);

    text.setString("Your final score: " + patch::to_string(global_score) );
    text.setCharacterSize(13);
    text.setColor(sf::Color(255, 255, 255));

    // center text
    b = text.getLocalBounds();
    text.setPosition(((S_WIDTH + 1) * 16 - b.width) / 2, ((S_HEIGHT + 3) * 16 - b.height) / 2 + 75);

    renderTexture.draw(text);
}

void draw_win_screen(void){
    sf::Text text;
    text.setString("You Win");
    //text.setPosition(10,10);
    text.setCharacterSize(50);
    text.setColor(sf::Color(0, 255, 0)); // todo animate colors
    text.setFont(font);

    // center text
    sf::FloatRect b = text.getLocalBounds();
    text.setPosition(((S_WIDTH + 1) * 16 - b.width) / 2, ((S_HEIGHT + 3) * 16 - b.height) / 2);

    renderTexture.draw(text);

    text.setString("Your final score: " + patch::to_string(global_score) );
    text.setCharacterSize(13);
    text.setColor(sf::Color(255, 255, 255));

    // center text
    b = text.getLocalBounds();
    text.setPosition(((S_WIDTH + 1) * 16 - b.width) / 2, ((S_HEIGHT + 3) * 16 - b.height) / 2 + 75);

    renderTexture.draw(text);
}

void draw_main_menu(){
    sf::Text text;
    text.setString("a_game");
    //text.setPosition(10,10);
    text.setCharacterSize(50);
    text.setColor(sf::Color(255, 255, 255)); // todo animate colors
    text.setFont(font);

    // center text
    sf::FloatRect b = text.getLocalBounds();
    text.setPosition(0, ((S_HEIGHT + 3) * 16) / 2 - b.height);

    renderTexture.draw(text);

    if (gen_idx == 0) {
        text.setString("-------------NEW");
        text.setColor(sf::Color(255, 255, 255));
    } else {
        text.setString("NEW");
        text.setColor(sf::Color(0, 255, 0));
    }
    text.setCharacterSize(25);
    b = text.getLocalBounds();
    text.setPosition((S_WIDTH * 16) - b.width, ((S_HEIGHT + 3) * 16) / 2 - b.height - 75); //50
    renderTexture.draw(text);

    if (gen_idx == 1) {
        text.setString("------------LOAD");
        text.setColor(sf::Color(255, 255, 255));
    } else {
        text.setString("LOAD");
        text.setColor(sf::Color(0, 255, 0));
    }
    b = text.getLocalBounds();
    text.setPosition((S_WIDTH * 16) - b.width, ((S_HEIGHT + 3) * 16) / 2 - b.height - 50); // 75
    renderTexture.draw(text);

    if (gen_idx == 2) {
        text.setString("------------SAVE");
        text.setColor(sf::Color(255, 255, 255));
    } else {
        text.setString("SAVE");
        text.setColor(sf::Color(0, 255, 0));
    }
    b = text.getLocalBounds();
    text.setPosition((S_WIDTH * 16) - b.width, ((S_HEIGHT + 3) * 16) / 2 - b.height - 25); //100
    renderTexture.draw(text);

    if (gen_idx == 3) {
        text.setString("-----------LEVEL");
        text.setColor(sf::Color(255, 255, 255));
    } else {
        text.setString("LEVEL");
        text.setColor(sf::Color(0, 255, 0));
    }
    b = text.getLocalBounds();
    text.setPosition((S_WIDTH * 16) - b.width, ((S_HEIGHT + 3) * 16) / 2 - b.height); //125
    renderTexture.draw(text);

    if (gen_idx == 4) {
        text.setString("----------CREDIT");
        text.setColor(sf::Color(255, 255, 255));
    } else {
        text.setString("CREDIT");
        text.setColor(sf::Color(0, 255, 0));
    }
    b = text.getLocalBounds();
    text.setPosition((S_WIDTH * 16) - b.width, ((S_HEIGHT + 3) * 16) / 2 - b.height + 25); //150
    renderTexture.draw(text);

    if (gen_idx == 5) {
        text.setString("---------OPTIONS");
        text.setColor(sf::Color(255, 255, 255));
    } else {
        text.setString("OPTIONS");
        text.setColor(sf::Color(0, 255, 0));
    }
    b = text.getLocalBounds();
    text.setPosition((S_WIDTH * 16) - b.width, ((S_HEIGHT + 3) * 16) / 2 - b.height + 50); //175
    renderTexture.draw(text);
}

void draw_new_game(){

}

void draw_load_game(){

}

void draw_save_game(){

}

void draw_level_select(){

}

void draw_credits(){

}

void draw_options(){

}
