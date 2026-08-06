/* sound_stop_impulse @0x83718B38 — request that a looping/impulse sound fade out and stop. No-op if the
 * sound system is down, the datum is gone, or the sound is not a one-shot impulse (sound_datum.type @+2
 * must be _sound_impulse). Otherwise it kicks off a fade-out of the sound over sound_player_fade_out_time
 * seconds via sound_start_fade. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/sound_datum.h"
#include "headers/sound_type.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);
extern void sound_start_fade(int16_t mode, float seconds, int fade_out_sound_index, int fade_up_sound_index);

void sound_stop_impulse(uint16_t sound_index)
{
    /* the raw read `*((__int16 *)elem + 1)` is sound_datum.type @+2 (DB: type @2) — not a flag word */
    if ( sound_data && datum_try_and_get(sound_data, sound_index)
         && DATA_ARRAY_ELEMENT(sound_data, sound_datum, sound_index)->type == _sound_impulse )
        sound_start_fade(0, sound_player_fade_out_time, -1, sound_index);
}
