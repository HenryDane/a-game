#include <SFML/Graphics.hpp>
#include "display.h"
#include "patch.h"
#include "main.h"

std::vector<sf::Texture> textures;
sf::Font font;

bool init_display(void){
    textures.clear();
    for (int i = 0; i < NUM_TEX_FILES; i++){
        sf::Texture tex;

        if (!tex.loadFromFile("res/" + patch::to_string(i) + ".png")) return (- 1000 - i);

        textures.push_back(tex);
    }

    if (!font.loadFromFile("res/telegrama_raw.ttf")) return -10;

    return true;
}

bool jumpwr_xyt(int x, int y, int t){ // tile coords
    sf::Sprite sprite1;
    sprite1.setTexture(textures.at(t));
    sprite1.setPosition(x * TILE_WIDTH, y * TILE_WIDTH);
    sprite1.setScale(TEX_SCALE, TEX_SCALE); // (128 / 4) = 32

    renderTexture.draw(sprite1);
    return true;
}

bool jumptex_xyt(int x, int y, std::string text){
    sf::Text text1;
    text1.setString(text);
    text1.setCharacterSize(TILE_WIDTH - 2);
    text1.setPosition( (x * TILE_WIDTH) + 3, (y * TILE_WIDTH) );
    text1.setFillColor(sf::Color(255, 255, 255));
    text1.setFont(font);

    renderTexture.draw(text1);
    return true;
}

bool jumptex_xyt(int x, int y, std::string text, sf::Color c){
    sf::Text text1;
    text1.setString(text);
    text1.setCharacterSize(TILE_WIDTH - 2);
    text1.setPosition( (x * TILE_WIDTH) + 3, (y * TILE_WIDTH));
    text1.setFillColor(c);
    text1.setFont(font);

    renderTexture.draw(text1);
    return true;
}

bool sf_line(float x1, float y1, float x2, float y2){
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(x1, y1)),
        sf::Vertex(sf::Vector2f(x2, y2))
    };

    renderTexture.draw(line, 2, sf::Lines);

    return true;
}

bool clear_display(void){
    renderTexture.clear();
    return true;
}

char get_dkey(void){

    return 0;
}
