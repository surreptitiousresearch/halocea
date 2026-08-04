/* unattached_impulse_sound_new @0x83713A80 — play a one-shot impulse sound at a fixed world location
 * (no owning object). Builds a world-spatialized sound_source at the given location and hands it to the
 * sound mixer. */

#include <stdint.h>
#include <stddef.h>   /* NULL */
#include "headers/sound_source.h"   /* sound_source + sound_location */
#include "headers/spatialization_mode.h"
extern void *memcpy(void *dst, const void *src, unsigned int size);

extern int sound_new_impulse(int definition_index, sound_source *source, int source_identifier, uint8_t (*track_proc)(int, const void *, sound_source *), const void *track_data, int16_t track_data_size, uint8_t is_local_player);

int unattached_impulse_sound_new(int definition_index, const sound_location *location, float scale,
                                 uint8_t is_player)
{
    sound_source source;

    source.scale = scale;
    memcpy(&source.location, location, sizeof(source.location));
    source.spatialization_mode = _sound_spatialization_mode_absolute;
    source.gain = 1.0f;
    return sound_new_impulse(definition_index, &source, -1, NULL, NULL, 0, is_player);
}
