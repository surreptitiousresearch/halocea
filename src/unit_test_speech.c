/* unit_test_speech @0x837BA6B0 — decide whether a unit may speak a given vocalization, resolving the concrete
 * sound and applying the speech-priority arbitration rules. Resolves the sound definition for the requested
 * vocalization type from the unit's dialogue tag, optionally walking the vocalization fallback chain
 * (dialogue_vocalization_lookup) when allow_recursive_lookup is set. Then, unless the unit is mute
 * (damage_flags bit 4, overridden by priority 10), compares the request priority against the unit's
 * current/queued speech priorities and the global override/queue tables.
 *
 * Returns: 0 = cannot speak, 1 = speak (queued/allowed), 2 = speak immediately (nothing playing / higher
 * override), 3 = speak (override priority outranks current). Writes the resolved vocalization type and sound
 * index back through the reference parameters, and the unit's last-speech time if requested. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_speech_priority.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);
extern int16_t dialogue_vocalization_lookup[];
extern int  global_speech_override_priorities[];
extern float global_speech_queue_times[];

int16_t unit_test_speech(int unit_index, int16_t priority, uint8_t allow_recursive_lookup,
                         uint8_t allow_queue, int *unit_last_speech_time,
                         int16_t *vocalization_type_reference, int *sound_definition_index_reference)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    game_time_get();
    int sound_index = *sound_definition_index_reference;
    int result = 0;
    int16_t vocalization_type = *vocalization_type_reference;

    if ( *sound_definition_index_reference == -1 )
    {
        int dialogue_tag = unit->unit.dialogue_index;
        if ( dialogue_tag != -1 )
        {
            for ( int16_t type = vocalization_type; vocalization_type != -1; type = vocalization_type )
            {
                sound_index = TAG_GET(int, dialogue_tag)[4 * type + 7];
                if ( !allow_recursive_lookup )
                    break;
                if ( sound_index != -1 )
                    break;
                vocalization_type = dialogue_vocalization_lookup[type];
            }
        }
    }

    if ( ((unit->object.damage_flags & (1u << _object_dead_bit)) == 0 || priority == _unit_speech_death) && sound_index != -1 )
    {
        int16_t current_priority = unit->unit.speech.current.priority;
        int16_t effective_priority = current_priority;
        if ( current_priority )
        {
            int16_t queued_priority = unit->unit.speech.queued.priority;
            if ( current_priority <= queued_priority )
                effective_priority = unit->unit.speech.queued.priority;

            /* a played line whose sound timer has expired can be pre-empted */
            if ( (priority == _unit_speech_pain || priority == _unit_speech_involuntary || priority == _unit_speech_death)
              && unit->unit.speech.played
              && !unit->unit.speech.sound_timer
              && priority > effective_priority )
            {
                current_priority = 0;
                effective_priority = unit->unit.speech.queued.priority;
            }

            int override_priority = global_speech_override_priorities[priority];
            if ( override_priority < effective_priority )
            {
                if ( priority < _unit_speech_involuntary || override_priority < current_priority )
                {
                    if ( allow_queue )
                    {
                        char can_queue;
                        float queue_time = global_speech_queue_times[priority];
                        if ( queue_time == 0.0 )
                            can_queue = 0;
                        else if ( queue_time == 3.4028235e38 )
                            can_queue = 1;
                        else
                        {
                            /* elapsed = post_delay_timer + sound_timer vs queue delay (ticks) */
                            unsigned int elapsed = unit->unit.speech.post_delay_timer
                                                 + unit->unit.speech.sound_timer;
                            int queue_ticks = (int)(global_speech_queue_times[priority] * 30.0f);
                            /* branchless-signed XOR idiom == signed ((__int16)queue_ticks > elapsed) */
                            can_queue = (int16_t)queue_ticks > (int)elapsed;
                        }

                        if ( can_queue )
                        {
                            if ( priority <= effective_priority )
                            {
                                if ( priority <= queued_priority )
                                {
                                    can_queue = 0;
                                }
                                else
                                {
                                    if ( current_priority == _unit_speech_pain || current_priority == _unit_speech_involuntary )
                                        can_queue = 1;
                                    if ( priority == _unit_speech_scripted )
                                        can_queue = 1;
                                }
                            }
                            else
                            {
                                can_queue = 1;
                            }
                        }
                        if ( can_queue )
                            result = 1;
                    }
                }
                else
                {
                    result = 2;
                }
            }
            else
            {
                result = 3;
            }
        }
        else
        {
            result = 2;
        }
    }

    *vocalization_type_reference = vocalization_type;
    *sound_definition_index_reference = sound_index;
    if ( unit_last_speech_time )
        *unit_last_speech_time = unit->unit.speech.last_speech_finished_time;
    return result;
}
