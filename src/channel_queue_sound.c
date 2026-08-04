/* channel_queue_sound @ 0x83715D40 — hand a permutation to the platform mixer for a channel and
 * record it in the channel datum. If something is already playing, the new permutation becomes the
 * queued one (released first if a stale queue entry existed); otherwise it becomes the playing one
 * and the channel's estimated playback tick resets. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_channel_datum.h"
#include "headers/sound_permutation.h"

extern void sound_cache_sound_finished(sound_permutation *sound);

void channel_queue_sound(int16_t channel_index, sound_permutation *permutation, int identifier,
                         uint8_t is_local_player, int16_t class_index, uint8_t lapping)
{
    sound_channel_datum *channel = &sound_channels[channel_index];

    if ( channel->queued_permutation )
        sound_cache_sound_finished(channel->queued_permutation);

    sound_manager_globals.platform->queue_sound_to_channel(channel_index, permutation, identifier,
                                                           is_local_player, class_index, lapping);

    if ( channel->playing_permutation )
    {
        channel->queued_permutation = permutation;
    }
    else
    {
        channel->playing_permutation = permutation;
        channel->estimated_tick_time = 0.0f;
    }
}
