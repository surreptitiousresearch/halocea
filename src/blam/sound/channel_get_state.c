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
extern uint8_t _sound_cache_sound_request(sound_permutation *sound, uint8_t block, uint8_t load, uint8_t reference);

int16_t channel_get_state(int16_t channel_index)
{
    sound_channel_datum *channel = &sound_channels[channel_index];
    /* DEVIATION: this was a `((int (*)(void))…)()` cast under a note claiming the bctrl at
     * 0x83715EFC "invokes the vtable slot with NO argument set up". It does — because it needs no
     * instruction: r3 still holds this function's own `channel_index` parameter, which nothing
     * writes between the prologue at 0x83715EC0 and the call (`extsh r11, r3` only reads it). The
     * argument is passed. DB types_members platform_sound_manager_definition.get_channel_state is
     * `__int16 (*)(__int16)` and funcs dsound_virtual_get_state takes one arg; the cast was a
     * fabricated calling convention and is dropped. */
    int state = sound_manager_globals.platform->get_channel_state(channel_index);
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
