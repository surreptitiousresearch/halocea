/* player_effect_update_screen_flash @0x83737F50 — merge a new screen-flash definition into a local
 * player's live effect state. The new flash is rejected when an existing higher-priority flash is still
 * running longer than the new one would, or when the flash type is not renderable. On acceptance the
 * definition is copied into the state block, its duration is pre-scaled by the per-tick time factor, and
 * the flash intensity is computed as the scale-interpolated value clamped to [0, max_intensity].
 *
 * DEVIATION: the DB prototype lists a leading __int16 local_player_index plus a separate definition
 * pointer; the disassembly shows r3 = the player_effect_datum* state, r4 = the screen_flash_definition*,
 * and scale/time_scale in f1/f2 — so the real signature is (effect_state, screen_flash, scale,
 * time_scale). The decompiler mis-typed the definition as a player_effect_datum, so its
 * direction/jitter/screen_flash members are the definition's priority / duration / type / intensities. */

#include <string.h>
#include "headers/player_effect_globals.h"
#include "headers/screen_flash_definition.h"
#include "headers/screen_flash_just_started_flags.h"
#include "headers/blam_data_globals.h"


void player_effect_update_screen_flash(player_effect_datum *effect_state,
                                       const screen_flash_definition *screen_flash,
                                       float scale, float time_scale)
{
    /* recovered: *(__int16 *)(state + 26) -> effect_state->screen_flash.priority */
    int current_priority = effect_state->screen_flash.priority;
    float time_factor = (time_scale * 30.0f);

    int apply = 1;
    if ((current_priority > screen_flash->priority
            && (float)effect_state->screen_flash_time_left > (double)(screen_flash->duration * time_factor))
        || !render_screen_flash_type_map[screen_flash->type])
    {
        apply = 0;
    }

    if (apply)
    {
        /* recovered: state + 24 -> &effect_state->screen_flash */
        memcpy(&effect_state->screen_flash, screen_flash, sizeof(screen_flash_definition));
        /* recovered: *(float *)(state + 40) -> effect_state->screen_flash.duration */
        float scaled_duration = effect_state->screen_flash.duration * time_factor;
        effect_state->screen_flash.duration = scaled_duration;
        effect_state->screen_flash_time_left = (int)scaled_duration;

        float intensity = 0.0f;
        float scaled_intensity =
            ((1.0f - screen_flash->zero_scale_factor) * scale) + screen_flash->zero_scale_factor;
        if (scaled_intensity >= 0.0f)
        {
            intensity = screen_flash->max_intensity;
            if (scaled_intensity <= screen_flash->max_intensity)
                intensity = scaled_intensity;
        }
        char flags = effect_state->flags;
        /* recovered: *(float *)(state + 60) -> effect_state->screen_flash.zero_scale_factor (reused as live intensity) */
        effect_state->screen_flash.zero_scale_factor = intensity;
        effect_state->flags = flags | (1u << _screen_flash_just_started_bit);
    }
}
