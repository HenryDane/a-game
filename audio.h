#include <SFML/Audio.hpp>

bool update_sound(void);
bool check_audio_state(void);
void skip_current_song(void);

void skip_seconds(int s);
void increase_volume(int v);
void decrease_volume(int v);

extern int current_music_state;
extern bool load_flag;
