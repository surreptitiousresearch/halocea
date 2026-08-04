/* ai_communication_finished @0x837CF890 — after a line is spoken, possibly trigger a scripted reply. Walks the
 * contiguous run of global_reply_table rows matching the just-spoken vocalization_type. For each, resolves the
 * replying unit (a preselected reply actor, or — by protagonist_type — a found ally / the original target / an
 * "other" team-mate), gates on damage category, scripted-dialog state, a per-row chance (unless forced by
 * reply_to_player) and an optional reply_filter callback, then if the reply unit can speak, voices the reply
 * (unit_speak), updates its reply timers and makes it glance back at the original speaker.
 *
 * Deviations: the calls to ai_communication_find_*_actor_to_talk and ai_communication_consider_speech have
 * trailing arguments mis-attributed by Hex-Rays (a float arg shuffles the GPR assignment); the true arguments
 * were recovered from the disassembly's per-register prototype annotations — in particular the talk-finder
 * animation_type comes from the reply row and the flags argument is 0 (r25), and consider_speech's last two
 * arguments are &sound_definition_index and a null debug string. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/dialogue_usage_flags.h"
#include "headers/actor_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/ai_globals.h"
#include "headers/reply_usage.h"
#include "headers/unit_speech_item.h"
#include "headers/object_type.h"
#include "headers/ai_communication_protagonist_type.h"
#include "headers/blam_data_globals.h"

extern const reply_usage global_reply_table[];
extern const __int16 communication_speech_priorities[];
extern const float   communication_notification_delays[];
extern const __int16 communication_protagonist_default_look_priorities[];

extern int ai_communication_find_global_actor_to_talk(int16_t team_index, int16_t find_actor_mode, int subject_unit_index, int cause_unit_index, float max_distance, int16_t ai_communication_type, int16_t ai_communication_priority, int16_t unit_speech_priority, int16_t vocalization_type, int16_t animation_type, int16_t flags);
extern int ai_communication_find_specific_actor_to_talk(int ai_index, int subject_unit_index, int cause_unit_index, float max_distance, int16_t ai_communication_type, int16_t ai_communication_priority, int16_t unit_speech_priority, int16_t vocalization_type, int16_t animation_type, int16_t flags);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t sound_scripted_dialog_is_playing(void);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern int16_t ai_communication_consider_speech(int unit_index, int16_t communication_priority, int16_t speech_priority, int16_t delay_ticks, uint8_t allow_vocalization_lookup, uint8_t allow_recent_disabling, int16_t *vocalization_type, float *weight, int *sound_definition_index_reference, char *debugstring);
extern void unit_speak(int unit_index, int16_t play_type, const unit_speech_item *speech_item);
extern void ai_communication_update_speech_timers(int unit_index, int16_t priority, int16_t vocalization_type, int16_t dialogue_type_index);
extern void ai_communication_look_secondary_at_unit(int actor_index, int16_t type, int16_t priority, int look_unit_index, int prop_index);

void ai_communication_finished(int unit_index, int16_t priority, int16_t vocalization_type,
                               uint8_t reply_to_player, int preselected_reply_actor_index,
                               ai_information_packet *ai_information)
{
    if ( !ai_globals->dialogue_triggers_enabled || vocalization_type == -1 )
        return;

    const reply_usage *row = global_reply_table;
    __int16 reply_row_index = 0;
    do
    {
        if ( row->original_vocalization_type != vocalization_type )
            goto next_row;

        unit_datum *unit =
            (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        actor_datum *speaker_actor = unit->unit.actor_index == -1 ? nullptr
                            : DATA_ARRAY_ELEMENT(actor_data, actor_datum, unit->unit.actor_index);
        __int16 speech_priority = communication_speech_priorities[row->communication_priority];

        if ( row->original_damage_category != -1
          && row->original_damage_category != ai_information->damage_category )
            goto next_row;
        if ( sound_scripted_dialog_is_playing() && (row->flags & (1u << _dialogue_usage_lookup_bit)) == 0 )
            goto next_row;

        int target_unit_index;
        if ( preselected_reply_actor_index != -1 )
        {
            actor_datum *preselected_actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, preselected_reply_actor_index);
            target_unit_index = preselected_actor->meta.unit_index;
        }
        else
        {
            int found_actor;
            switch ( (unsigned __int16)row->protagonist_type )
            {
                case _comm_protagonist_friend:
                    if ( !speaker_actor || speaker_actor->meta.encounter_index == -1 )
                        found_actor = ai_communication_find_global_actor_to_talk(
                            unit->object.owner_team_index, 1, unit_index, -1, 9.0f, row->vocalization_type, -1,
                            row->communication_priority, speech_priority, row->animation_type, 0);
                    else
                        found_actor = ai_communication_find_specific_actor_to_talk(
                            (unsigned __int16)speaker_actor->meta.encounter_index, unit_index, -1, 9.0f,
                            row->animation_type, -1, row->communication_priority, speech_priority,
                            row->vocalization_type, 0);
                    if ( found_actor == -1 )
                        goto next_row;
                    {
                        actor_datum *found_actor_datum = DATA_ARRAY_ELEMENT(actor_data, actor_datum, found_actor);
                        target_unit_index = found_actor_datum->meta.unit_index;
                    }
                    break;
                case _comm_protagonist_target:
                    if ( !object_try_and_get_and_verify_type(ai_information->target_unit_index, object_mask_unit) )
                        goto next_row;
                    target_unit_index = ai_information->target_unit_index;
                    break;
                case _comm_protagonist_enemy:
                    found_actor = ai_communication_find_global_actor_to_talk(
                        unit->object.owner_team_index, 2, unit_index, -1, 9.0f, row->vocalization_type, -1,
                        row->communication_priority, speech_priority, row->animation_type, 0);
                    if ( found_actor == -1 )
                        goto next_row;
                    {
                        actor_datum *found_actor_datum = DATA_ARRAY_ELEMENT(actor_data, actor_datum, found_actor);
                        target_unit_index = found_actor_datum->meta.unit_index;
                    }
                    break;
                default:
                    goto next_row;
            }
        }

        if ( target_unit_index == -1 )
            goto next_row;

        unit_datum *target =
            (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, target_unit_index)->datum;
        if ( target->unit.player_index != -1 )
            goto next_row;

        unsigned __int8 do_reply = reply_to_player;
        if ( !reply_to_player && row->chance > 0.0f )
        {
            unsigned int *seed = get_global_random_seed_address();
            if ( real_seed_random(seed) < row->chance )
                do_reply = 1;
        }
        if ( !do_reply )
            goto next_row;

        if ( row->reply_filter
          && !row->reply_filter(unit_index, ai_information, target->unit.actor_index) )
            goto next_row;

        __int16 reply_vocalization = row->vocalization_type;
        __int16 delay_ticks = (int)(row->delay_time * 30.0f);
        float weight = 1.0f;
        int sound_definition_index = -1;
        __int16 considered_vocalization = ai_communication_consider_speech(
            target_unit_index, row->communication_priority, speech_priority, delay_ticks, 0, 0,
            &reply_vocalization, &weight, &sound_definition_index, nullptr);
        if ( considered_vocalization <= 0 )
            goto next_row;

        unit_speech_item speech;
        speech.priority = speech_priority;
        speech.delay_time = delay_ticks;
        speech.ai.target_unit_index = unit_index;
        speech.sound_definition_index = sound_definition_index;
        speech.vocalization_type = reply_vocalization;
        speech.pause_time = 24;
        speech.ai.communication_type = -1;
        speech.ai.dialogue_type_index = -1;
        speech.ai.damage_category = -1;
        speech.ai.updated_dialogue_timers = 1;
        speech.ai.look_priority = 0;
        speech.ai.look_type = 0;
        speech.ai.information_type = 0;
        speech.ai_notification_delay = (int)(communication_notification_delays[row->communication_priority] * 30.0f);
        for ( unsigned i = 0; i < sizeof(speech.ai.information_data); ++i )
            ((unsigned char *)&speech.ai.information_data)[i] = 0;  /* byte-wise clear; DB union has no raw view */

        unit_speak(target_unit_index, considered_vocalization, &speech);
        ai_communication_update_speech_timers(target_unit_index, speech_priority, -1, reply_row_index); /* phantom 5th arg dropped */
        ai_communication_look_secondary_at_unit(target->unit.actor_index, 8,
            communication_protagonist_default_look_priorities[row->communication_priority], unit_index, -1);
        return;

next_row:
        ++row;
        ++reply_row_index;
    }
    while ( (unsigned __int16)row->original_vocalization_type != 0xFFFF );
}
