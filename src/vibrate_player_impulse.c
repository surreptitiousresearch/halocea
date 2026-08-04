/* vibrate_player_impulse @0x83739198 — start a controller rumble impulse for a local player from a vibrate
 * definition. The player's eight concurrent impulse slots are scanned for the one with the largest elapsed
 * time (the oldest, safest to overwrite); the definition is copied into it, its two channel durations are
 * scaled by time_scale, and its two channel amplitudes are scaled by a blend between the definition's
 * zero-scale floor and full strength according to scale. The chosen slot's elapsed time is reset to zero.
 *
 * DEVIATION: scale/time_scale are floats (rendered as doubles by the FPR-shadow ABI); the slot-selection
 * unrolled if-ladder is restored to a loop over the eight impulse slots. */

#include <stdint.h>
#include "headers/vibrate_global_data.h"
#include "headers/blam_data_globals.h"

extern void *memcpy(void *dest, const void *src, unsigned int count);


void vibrate_player_impulse(int16_t local_player_index, const vibrate_definition *definition, float scale,
                            float time_scale)
{
    player_vibrate_datum *player = &vibrate_globals->player_vibrate_data[local_player_index];

    int chosen = 0;
    float max_elapsed = player->time_elapsed[0];
    for (int i = 1; i < 8; ++i)
    {
        if (player->time_elapsed[i] > max_elapsed)
        {
            chosen = i;
            max_elapsed = player->time_elapsed[i];
        }
    }

    vibrate_definition *slot = &player->impulse[chosen];
    memcpy(slot, definition, sizeof(vibrate_definition));

    float zero_scale_factor = definition->zero_scale_factor;
    float amplitude_blend = (1.0f - zero_scale_factor) * scale + zero_scale_factor;

    slot->vibrate_frequencies[0].duration *= time_scale;
    slot->vibrate_frequencies[1].duration *= time_scale;
    slot->vibrate_frequencies[0].frequency *= amplitude_blend;
    slot->vibrate_frequencies[1].frequency *= amplitude_blend;

    player->time_elapsed[chosen] = 0.0f;
}
