/* cinematic_start @0x83690EC8 — begin a cinematic: notify the HCEX host, save and max out the music gain,
 * disable player input and AI dialogue triggers, raise the letterbox, mark the cinematic in progress, and
 * clear all live projectiles. */

#include <stdint.h>
#include "headers/cinematic_globals.h"
#include "headers/blam_data_globals.h"

extern void hcex_fire_event(const char *name);
extern void hcex_start_cine(void);
extern float sound_get_music_gain(void);
extern void sound_set_music_gain(float gain);
extern void player_input_enable(uint8_t enable);
extern void ai_globals_dialogue_triggers_enabled(uint8_t enable);
extern int game_time_get(void);
/* extern corrected to match def (projectiles_delete_all.c): returns void* (ignored iterator result). */
extern void projectiles_delete_all(void);

void cinematic_start(void)
{
    hcex_fire_event("cinameticStart");
    hcex_start_cine();
    if ( music_gain_in_profile == -1.0f )
        music_gain_in_profile = sound_get_music_gain();
    sound_set_music_gain(1.0f);
    player_input_enable(0);
    ai_globals_dialogue_triggers_enabled(0);
    cinematic_globals->letter_box = 1;
    cinematic_globals->letter_box_last_game_time = game_time_get();
    cinematic_globals->cinematic_in_progress = 1;
    projectiles_delete_all();
}
