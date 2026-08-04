/* unspatialized_impulse_sound_new @ 0x83713B08 — fire a one-shot 2D (unspatialized) sound at
 * the given scale and full gain. */

#include <stdint.h>
#include "headers/sound_source.h"
#include "headers/spatialization_mode.h"

extern int sound_new_impulse(int definition_index, sound_source *source, int source_identifier, uint8_t (*track_proc)(int, const void *, sound_source *), const void *track_data, int16_t track_data_size, uint8_t is_local_player);

int unspatialized_impulse_sound_new(int definition_index, float scale)
{
    sound_source source;

    source.scale = scale;
    source.spatialization_mode = _sound_spatialization_mode_none;
    source.gain = 1.0f;
    return sound_new_impulse(definition_index, &source, -1, 0, 0, 0, 0);
}
