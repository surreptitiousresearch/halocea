/* prioritize_sounds @ 0x8371ABA0 — assign hardware channels to the active sounds whose start time has
 * arrived. For each due sound: if it already owns a channel, or its permutation can be brought into
 * the cache, it is marked cache-requested, given a channel (evicting any lower-priority sound there),
 * and (re)stamped with the current time. A sound that cannot be cached is stopped — unless it is
 * already channelled, is a tracked looping-impulse sound, or its class defers on cache miss; in the
 * stop case the permutation is also recorded in its pitch range's cache-miss slot for later retry. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_channel_datum.h"
#include "headers/sound_datum.h"
#include "headers/sound_definition.h"
#include "headers/sound_pitch_range.h"
#include "headers/sound_class_definition.h"
#include "headers/sound_permutation.h"
#include "headers/sound_flags.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


#include "headers/loop_impulse_sound_tracking_data.h"
#include "headers/sound_source.h"
#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern uint8_t _sound_cache_sound_request(sound_permutation *sound, uint8_t block, uint8_t load, uint8_t reference);
extern int16_t sound_find_channel(uint16_t sound_index);
extern void sound_stop(int sound_index);
/* declared to the generic callback-slot signature (matches sound_datum.track_proc) so the
 * funcptr identity comparison below needs no cast */
extern uint8_t track_loop_impulse_sound(int sound_index, const void *track_data, sound_source *source);

void prioritize_sounds(void)
{
    int i = data_next_index(sound_data, -1);

    while ( i != -1 )
    {
        sound_datum *datum = DATA_ARRAY_ELEMENT(sound_data, sound_datum, i);

        if ( datum->start_time <= sound_manager_globals.render_time )
        {
            int assigned = (uint16_t)datum->playing_channel_index != 0xFFFF;
            sound_definition *def = TAG_GET(sound_definition, datum->definition_index);
            sound_permutation *permutation =
                &((sound_permutation *)((sound_pitch_range *)def->pitch_ranges.address
                                        + datum->pitch_range_index)->permutations.address)[datum->permutation_index];
            int do_stop = 1;

            if ( assigned || _sound_cache_sound_request(permutation, 0, 1u, 1u) )
            {
                int16_t channel;
                datum->flags |= (1u << _sound_cached_bit);
                channel = sound_find_channel(i);
                if ( channel != -1 )
                {
                    int existing = sound_channels[channel].sound_index;
                    if ( existing != i )
                    {
                        if ( existing != -1 )
                            sound_stop(existing);
                        sound_channels[channel].sound_index = i;
                        datum->start_time = sound_manager_globals.render_time;
                    }
                    do_stop = 0;
                }
            }
            else if ( assigned
                   || datum->track_proc == track_loop_impulse_sound
                   || sound_classes[def->class_index].cache_miss_mode )
            {
                do_stop = 0;
            }
            else
            {
                sound_pitch_range *pitch_range =
                    (sound_pitch_range *)def->pitch_ranges.address + datum->pitch_range_index;
                if ( (uint16_t)pitch_range->runtime_discarded_permutation_index == 0xFFFF )
                    pitch_range->runtime_discarded_permutation_index = datum->permutation_index;
            }

            if ( do_stop )
                sound_stop(i);
        }

        i = data_next_index(sound_data, i);
    }
}
