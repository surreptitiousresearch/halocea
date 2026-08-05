/* cinematic_stop @0x836917C8 — end a cinematic: restore the saved music gain, lower the letterbox, re-enable
 * player input and AI dialogue, reset screen effects and the ambient reflection tint, clear the near-clip
 * override, flush deferred error display, and notify the HCEX host. */

#include <stdint.h>
#include "headers/cinematic_globals.h"
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"

extern void sound_set_music_gain(float gain);
extern void player_input_enable(uint8_t enable);
extern void ai_globals_dialogue_triggers_enabled(uint8_t enable);
extern void rasterizer_screen_effects_initialize_for_new_map(void);
extern real_argb_color *global_rasterizer_model_ambient_reflection_tint;
extern void rasterizer_set_near_clip_distance(float near_clip_distance);
extern void display_errors_deferred_until_cinematic_stop(void);
extern void hcex_fire_event(const char *name);
extern int hcex_stop_cine(void);

void cinematic_stop(void)
{
    if ( music_gain_in_profile != -1.0f )
        sound_set_music_gain(music_gain_in_profile);
    music_gain_in_profile = -1.0f;
    cinematic_globals->letter_box = 0;
    player_input_enable(1u);
    ai_globals_dialogue_triggers_enabled(1u);
    cinematic_globals->cinematic_in_progress = 0;
    rasterizer_screen_effects_initialize_for_new_map();

    real_argb_color *ambient_tint = global_rasterizer_model_ambient_reflection_tint;
    if ( ambient_tint )
    {
        ambient_tint->n[0] = 0.0f;
        ambient_tint->n[1] = 0.0f;
        ambient_tint->n[2] = 0.0f;
        ambient_tint->n[3] = 0.0f;
    }

    rasterizer_set_near_clip_distance(0.0f);
    display_errors_deferred_until_cinematic_stop();
    hcex_fire_event("cinameticStop");
    hcex_stop_cine();
}
