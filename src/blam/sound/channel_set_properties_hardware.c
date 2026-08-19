/* channel_set_properties_hardware @0x83715DF0 — push a channel's resolved properties into the platform
 * sound backend: latch the pitch into the channel record (unless gain_only), duck weapon-fire-class
 * (_sound_class_weapon_fire == 4) channels whose sound datum is flagged is_local_player (+0xAC) —
 * gain boosted 1.2x then capped at 0.6 — and tail-call the platform's set_channel_properties hook.
 *
 * DEVIATION (bug class 6): the decompiler rendered the bctr tail call zero-arg; disasm shows all five
 * vtable args (channel_index, properties, gain_only, the alternate flag, class_index) are live. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/sound_channel_datum.h"
#include "headers/sound_manager_globals.h"
#include "headers/platform_sound_channel_properties.h"
#include "headers/sound_class.h"
#include "headers/sound_datum.h"
#include "headers/blam_data_globals.h"


void channel_set_properties_hardware(int16_t channel_index, platform_sound_channel_properties *properties, uint8_t gain_only, int16_t class_index)
{
    sound_channel_datum *channel = &sound_channels[channel_index];

    /* recovered: (uint8_t *)sound_data->data + 176*idx + 172 -> sound_datum.is_local_player
     * (176 = sizeof(sound_datum), +172 = 0xAC; DB types_members confirms the member). */
    uint8_t is_local_player = 0;
    if ( channel->sound_index != -1 )
        is_local_player = DATA_ARRAY_ELEMENT(sound_data, sound_datum,
                                             channel->sound_index)->is_local_player;

    if ( !gain_only )
        channel->pitch = properties->pitch;

    if ( is_local_player && class_index == _sound_class_weapon_fire )
    {
        float ducked = properties->gain * 1.2;
        if ( ducked > 0.6 )
            ducked = 0.6f;
        properties->gain = ducked;
    }

    sound_manager_globals.platform->set_channel_properties(channel_index, properties, gain_only,
                                                           is_local_player, class_index);
}
