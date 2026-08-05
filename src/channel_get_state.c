/* channel_get_state @ 0x83715EC0 — poll the platform mixer for a channel's playback state and
 * advance the channel's software queue accordingly. When the hardware has consumed the playing
 * permutation (state < 2) and a queued one exists, the queued permutation is promoted to playing
 * (re-requesting it from cache; if that fails the state is forced to 0). When nothing is left
 * playing (state < 1), the playing permutation is released. The channel's estimated playback tick
 * is integrated by pitch * elapsed ticks. Returns the resolved state. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_channel_datum.h"
#include "headers/sound_permutation.h"

extern void sound_cache_sound_finished(sound_permutation *sound);
extern int _sound_cache_sound_request(sound_permutation *permutation, int block, int load, int reference);

int16_t channel_get_state(int16_t channel_index)
{
    sound_channel_datum *channel = &sound_channels[channel_index];
    /* Faithful original-binary calling-convention pun: disasm 0x83715EFC (bctrl) invokes the vtable
     * slot with NO argument set up and reads a short return, even though the installed function
     * (dsound_virtual_get_state, short(short)) declares a channel-index parameter. The funcptr cast
     * reproduces the binary's short(void) invocation; the vtable member type stays correct. Kept. */
    int state = ((int (*)(void))sound_manager_globals.platform->get_channel_state)();
    sound_permutation *playing_permutation;

    if ( channel->queued_permutation && (int16_t)state < 2 )
    {
        sound_permutation *queued_permutation;
        sound_cache_sound_finished(channel->playing_permutation);
        queued_permutation = channel->queued_permutation;
        channel->queued_permutation = 0;
        channel->playing_permutation = queued_permutation;
        channel->estimated_tick_time = 0.0f;
        state = _sound_cache_sound_request(queued_permutation, 0, 0, 0) == 0 ? 0 : state;
    }

    playing_permutation = channel->playing_permutation;
    if ( playing_permutation && (int16_t)state < 1 )
    {
        sound_cache_sound_finished(playing_permutation);
        channel->playing_permutation = 0;
    }

    channel->estimated_tick_time =
        (channel->pitch * sound_manager_globals.ticks_elapsed) + channel->estimated_tick_time;
    return state;
}
