/* player_effect_update_camera_shake @0x83738430 — merge a new camera-shake definition into a local
 * player's live effect state. The new shake replaces the current one when it is stronger (its magnitude
 * exceeds the current impulse) or its scale dominates the current scale. On replacement the definition is
 * copied into the state block and its magnitude/period fields are pre-scaled by the per-tick time factor.
 *
 * DEVIATION: the DB prototype lists a leading __int16 local_player_index plus a separate definition
 * pointer; the disassembly shows r3 = the player_effect_datum* state, r4 = the camera_shake_definition*,
 * and scale/time_scale in f1/f2 — so the real signature is (effect_state, camera_shake, scale,
 * time_scale) with the int index a float-slot-ABI phantom. The decompiler also mis-typed the definition
 * as a player_effect_datum, so its `direction.n[0]`/`screen_flash.duration` are the definition's
 * duration / zero_scale_factor. */

#include <string.h>
#include "headers/player_effect_globals.h"
#include "headers/camera_shake_definition.h"
#include "headers/screen_flash_just_started_flags.h"

extern int game_time_get(void);

void player_effect_update_camera_shake(player_effect_datum *effect_state,
                                       const camera_shake_definition *camera_shake,
                                       float scale, float time_scale)
{
    game_time_get();

    float current_impulse = (float)effect_state->camera_shake_time_left;
    float time_factor = (time_scale * 30.0f);
    float new_magnitude = camera_shake->duration * time_factor;
    float new_scale = ((1.0f - camera_shake->zero_scale_factor) * scale) + camera_shake->zero_scale_factor;
    float old_scale = effect_state->camera_shake.zero_scale_factor;

    if (current_impulse < new_magnitude || new_scale > old_scale ||
        (new_scale >= old_scale && current_impulse < new_magnitude))
    {
        memcpy(&effect_state->camera_shake, camera_shake, sizeof(camera_shake_definition));
        float scaled_magnitude = effect_state->camera_shake.duration * time_factor;
        char flags = effect_state->flags;
        float scaled_period = effect_state->camera_shake.periodic_period * time_factor;
        effect_state->camera_shake.zero_scale_factor = new_scale; /* reused as live scale */
        effect_state->camera_shake.duration = scaled_magnitude;
        effect_state->flags = flags | (1u << _camera_shake_just_started_bit);
        effect_state->camera_shake.periodic_period = scaled_period;
        effect_state->camera_shake_time_left = (int)scaled_magnitude;
    }
}
