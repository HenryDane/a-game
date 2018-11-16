#include <SFML/Audio.hpp>
#include <iostream>

sf::Music music;

/*
 0 - title music
 1 - silence
 2 - music a (slow)
 3 - music b (med)
 4 - music c (fast)
*/
int current_music_state = 0;
bool load_flag = true;

bool update_sound(void){
    if (!load_flag) return true;

    int a = 1;

    switch (current_music_state){
    case 0:
        a = music.openFromFile("res/title.wav");
        break;
    case 1:
        music.stop();
        break;
    case 2:
        a = music.openFromFile("res/fast.wav");
        break;
    case 3:
        a = music.openFromFile("res/fast1.wav");
        break;
    case 4:
        a = music.openFromFile("res/fast2.wav");
        break;
    default:
        std::cout << "BAD SOUND STATE!" << std::endl;
        return false;
    }

    if (!a)
        return false;

    music.play();

    return true;
}

void skip_current_song(void){
    music.stop();

    if (current_music_state == 0) {
        current_music_state = 2;
    } else {
        current_music_state++;
        if (current_music_state == 5)
            current_music_state = 2;
    }

    update_sound();
}

bool check_audio_state() {
    if (current_music_state == 1) return false;
    if (music.getStatus() != sf::Sound::Stopped) return false;

    skip_current_song();

    return true;
}

void skip_seconds(int s){
    music.pause();
    auto newPos = music.getPlayingOffset() + sf::seconds(s);
    music.setPlayingOffset(std::min(music.getDuration(), sf::Time(newPos)));
    music.play();
}

void decrease_volume(int v) {
    music.setVolume(std::max(0.0f, music.getVolume() - v));
}

void increase_volume(int v){
    music.setVolume(std::min(100.0f, music.getVolume() + v));
}
