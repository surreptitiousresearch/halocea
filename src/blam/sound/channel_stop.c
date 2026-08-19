/* channel_stop @0x83715FA8 — retire a sound channel's queued and playing permutations (notifying the sound
 * cache each is finished), then tell the platform layer to stop the hardware channel. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"

#include "headers/sound_permutation.h"
extern void sound_cache_sound_finished(sound_permutation *sound);

void channel_stop(int16_t channel_index)
{
    sound_channel_datum *channel = &sound_channels[channel_index];

    if ( channel->queued_permutation )
    {
        sound_cache_sound_finished(channel->queued_permutation);
        channel->queued_permutation = nullptr;
    }

    if ( channel->playing_permutation )
    {
        sound_cache_sound_finished(channel->playing_permutation);
        channel->playing_permutation = nullptr;
    }

    sound_manager_globals.platform->stop_channel(channel_index);
}
