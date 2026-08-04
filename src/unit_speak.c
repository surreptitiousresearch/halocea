/* unit_speak @0x837BABA0 — commit a resolved speech item to a unit's speech state. play_type 1 queues the
 * item into the unit's queued-speech slot; play_type >= 2 installs it as the current/active speech and
 * (re)initialises the playback bookkeeping — clearing the played/notified/finished latches, loading the
 * pre-delay / ai-delay / post-delay timers from the item's delay_time / ai_notification_delay / pause_time,
 * resetting the impulse sound index sentinel (-1), and setting the line duration in ticks (sound_timer):
 * 45 when there is no sound, else 30 * sound_definition.duration_ms / 1000. A play_type 3 (override) also
 * clears any pending queued item. The whole thing is skipped when the unit is mute (damage_flags bit 4),
 * unless the priority is the forced value 10. */

#include <string.h>
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/sound_definition.h"
#include "headers/unit_speech_item.h"
#include "headers/unit_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_speech_priority.h"
#include "headers/unit_play_speech_type.h"
#include "headers/blam_data_globals.h"


void unit_speak(int unit_index, int16_t play_type, const unit_speech_item *speech_item)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit_speech *speech = &unit->unit.speech;

    if ( (unit->object.damage_flags & (1u << _object_dead_bit)) != 0 && speech_item->priority != _unit_speech_death )
        return;

    if ( play_type < _unit_play_speech_immediate )
    {
        if ( play_type == _unit_play_speech_queue )
            memcpy(&speech->queued, speech_item, 0x30u);
        return;
    }

    memcpy(&speech->current, speech_item, 0x30u);
    if ( play_type == _unit_play_speech_immediate_dequeue && speech->queued.priority > 0 )
        speech->queued.priority = 0;

    __int16 pre_delay  = speech->current.delay_time;
    __int16 post_delay = speech->current.pause_time;
    __int16 ai_delay   = speech->current.ai_notification_delay;
    int sound_definition_index = speech->current.sound_definition_index;

    speech->played = 0;
    speech->notified_ai = 0;
    speech->finished = 0;
    speech->impulse_sound_index = -1;
    speech->pre_delay_timer = pre_delay;
    speech->post_delay_timer = post_delay;
    speech->ai_delay_timer = ai_delay;

    if ( sound_definition_index == -1 )
        speech->sound_timer = 45;
    else
        speech->sound_timer =
            30 * TAG_GET(sound_definition, sound_definition_index)->runtime_maximum_play_time / 1000;   /* sound+132 */
}
