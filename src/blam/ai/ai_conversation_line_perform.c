/* ai_conversation_line_perform @0x837CDAA0 */
/* ai_conversation_line_perform @0x83? — advance one line of a running AI conversation, returning its "done"
 * flag. While not yet started: if no required participant is still busy talking/looking (and no scripted dialog
 * is already playing), the line is begun — either as a bare scripted sound (no speaker, or forced) or by asking
 * the speaking unit to speak it (building a unit_speech_item and calling unit_speak). If a participant is busy,
 * the start is deferred. Once started, the line is watched until its sound finishes; then an optional inter-line
 * delay counts down, the done flag is set, and looping lines (flag 8) toggle their repeat state.
 *
 * Conversation datum fields are accessed by offset (100-byte stride); participant actor indices live in the
 * datum at +40.
 *
 * DEVIATION: the decompiler rendered the "line finished" test as
 * `state - 6 - (state - 7 + (state == 6))`, which reduces to `(state != 6)`; written directly. */

#include <stdint.h>
#include <string.h>
#include "headers/conversation_datum.h"
#include "headers/unit_datum.h"
#include "headers/ai_conversation.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/scenario.h"
#include "headers/unit_speech_item.h"
#include "headers/actor_action.h"
#include "headers/conversation_line_flags.h"
#include "headers/ai_communication_priority.h"
#include "headers/ai_information_type.h"
#include "headers/ai_information_look_type.h"
#include "headers/blam_data_globals.h"


extern uint8_t sound_scripted_dialog_is_playing(void);
extern void scripted_sound_new(int definition_index, int source_object_index, float scale);
extern int scripted_sound_time(int definition_index);
extern int16_t unit_test_speech(int unit_index, int16_t priority, uint8_t allow_recursive_lookup, uint8_t allow_queue, int *unit_last_speech_time, int16_t *vocalization_type_reference, int *sound_definition_index_reference);
extern void unit_speak(int unit_index, int16_t play_type, const unit_speech_item *speech_item);

uint8_t ai_conversation_line_perform(int conversation_index)
{
    conversation_datum *conversation = DATUM_GET(conversation_data, conversation_datum, conversation_index);
    if (conversation->line_advance)
        return conversation->line_advance;

    if (!conversation->line_spoken)
    {
        uint8_t busy = 0;
        int sound_definition_index = conversation->line_sound_index;

        if (sound_definition_index != -1)
        {
            int16_t flags = conversation->line_flags;
            if ((flags & (1u << _ai_conversation_line_wait_until_speaker_nearby_bit)) != 0
                || (flags & (1u << _ai_conversation_line_wait_until_everyone_nearby_bit)) != 0)
            {
                int participant_count = ((ai_conversation *)global_scenario->ai_conversations.address
                                      + conversation->conversation_definition_index)->participants.count;
                for (int i = 0; i < participant_count; i = (int16_t)(i + 1))
                {
                    int participant_actor = conversation->actor_indices[i];
                    if (participant_actor == -1)
                        continue;
                    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, participant_actor);
                    if (((flags & (1u << _ai_conversation_line_wait_until_everyone_nearby_bit)) != 0
                         || ((flags & (1u << _ai_conversation_line_wait_until_speaker_nearby_bit)) != 0 && participant_actor == conversation->line_actor_index))
                        && actor->state.action == actor_action_converse
                        && actor->state.action_data.___u0.converse.run_to_unit_index != -1
                        && !actor->state.action_data.___u0.converse.in_range
                        && !actor->state.action_data.___u0.converse.failed)
                    {
                        busy = 1;
                    }
                }
            }

            if (sound_scripted_dialog_is_playing())
                busy = 1;

            if (!busy)
            {
                int speaking_unit_index = conversation->line_unit_index;
                if (speaking_unit_index == -1 || conversation->line_unspatialized)
                {
                    scripted_sound_new(conversation->line_sound_index, -1, 1.0f);
                }
                else
                {
                    int sound_definition = conversation->line_sound_index;
                    int16_t vocalization = -1;
                    int16_t play_handle = unit_test_speech(speaking_unit_index, _ai_communication_priority_yell, 0, 1, nullptr,
                                                           &vocalization, &sound_definition);
                    if (play_handle == 1)
                    {
                        busy = 1;
                    }
                    else if (play_handle > 0)
                    {
                        unit_speech_item speech_item;
                        memset(&speech_item, 0, sizeof(speech_item));
                        speech_item.priority = _ai_communication_priority_yell;
                        speech_item.vocalization_type = -1;
                        speech_item.ai.look_data.___u0.unit.unit_index = speaking_unit_index;
                        speech_item.sound_definition_index = conversation->line_sound_index;
                        speech_item.ai.target_unit_index = conversation->line_address_unit_index;
                        speech_item.ai.communication_type = -1;
                        speech_item.ai.damage_category = -1;
                        speech_item.ai.dialogue_type_index = -1;
                        speech_item.ai.look_priority = 1;
                        speech_item.ai.look_type = _ai_information_look_unit;
                        speech_item.ai.information_type = _ai_information_none;
                        unit_speak(speaking_unit_index, play_handle, &speech_item);
                    }
                }
            }
        }

        if (!busy)
        {
            conversation->line_spoken = 1;
            conversation->any_line_spoken = 1;
        }
    }

    if (conversation->line_spoken)
    {
        if (!conversation->line_finished)
        {
            if (conversation->line_unit_index == -1)
            {
                int sound_definition_index = conversation->line_sound_index;
                conversation->line_finished = (sound_definition_index == -1
                                    || scripted_sound_time(sound_definition_index) == 0);
            }
            else
            {
                int speaking_unit = conversation->line_unit_index;
                unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, speaking_unit)->datum);
                conversation->line_finished = ((uint16_t)unit_object->unit.speech.current.priority != _ai_communication_priority_yell);
            }
        }

        if (conversation->line_finished)
        {
            int16_t delay_ticks = conversation->line_delay_timer;
            if (delay_ticks > 0)
            {
                uint8_t result = conversation->line_advance;
                conversation->line_delay_timer = delay_ticks - 1;
                return result;
            }

            int16_t line_flags = conversation->line_flags;
            conversation->line_advance = 1;
            if ((line_flags & (1u << _ai_conversation_line_wait_after_until_told_to_advance_bit)) != 0)
            {
                if (!conversation->waiting_to_advance)
                {
                    conversation->waiting_to_advance = 1;
                    conversation->told_to_advance = 0;
                }
                if (conversation->told_to_advance)
                {
                    conversation->waiting_to_advance = 0;
                    return conversation->line_advance;
                }
                conversation->line_advance = 0;
            }
        }
    }

    return conversation->line_advance;
}
