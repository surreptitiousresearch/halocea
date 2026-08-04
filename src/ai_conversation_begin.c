/* ai_conversation_begin @0x837CEC90 — attempt to start a runtime conversation: fill every participant
 * slot, validate the conversation's trigger conditions, and on success switch the placed actors into the
 * "converse" action and mark the conversation begun.
 *
 * Two participant passes run first: a primary pass places non-secondary participants (flag 4 clear), and
 * an alternate pass places secondary participants (flag 4 set) when the primary pass requested alternates.
 * The conversation then fails-but-keeps-trying (sets *continue_trying when the definition is "persistent",
 * flag 0x40) if a required participant is missing, the triggering player is out of trigger range
 * (flag 0x40 distance gate), no participant is line-of-sight visible to / from a player (flags 0x10 / 0x80),
 * etc. On success it binds attach-object names, optionally runs action_converse_setup + actor_action_change
 * (flag 0x20), updates each unit's current dialogue variant, and sets conversation->begun.
 *
 * Returns 1 if the conversation began, 0 otherwise (writing *continue_trying when it should be retried). */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/conversation_participant_definition_flags.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/prop_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/scenario.h"
#include "headers/conversation_datum.h"
#include "headers/ai_conversation.h"
#include "headers/ai_conversation_definition_flags.h"
#include "headers/ai_conversation_participant.h"
#include "headers/actor_datum.h"
#include "headers/action_state_data.h"
#include "headers/converse_state_data.h"
#include "headers/actor_action.h"
#include "headers/data_iterator.h"
#include "headers/real_point3d.h"
#include "headers/prop_perception_state.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/converse_state_data.h"
extern uint8_t ai_conversation_find_participant(uint16_t conversation_index, int16_t participant_index, uint8_t *found_specific_unit_reference, uint8_t *try_alternate_reference, uint8_t *success_with_better_player_rating_reference, float *best_distance_reference);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern int prop_get_active_by_unit_index(uint16_t actor_index, int unit_index);
extern uint8_t unit_can_see_point(int unit_index, const real_point3d *point, float theta);
extern void object_set_object_index_for_name_index(int16_t name_index, int object_index);
extern uint8_t action_converse_setup(uint16_t actor_index, int conversation_index, converse_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);

uint8_t ai_conversation_begin(uint16_t conversation_index, uint8_t *continue_trying)
{
    conversation_datum *conversation =
        DATA_ARRAY_ELEMENT(conversation_data, conversation_datum, conversation_index);
    char *conversations_base = (char *)global_scenario->ai_conversations.address;
    ai_conversation *definition = (ai_conversation *)&conversations_base[116 * conversation->conversation_definition_index];

    float best_distance = 3.4028235e38f;
    unsigned __int8 try_alternate = 0;
    unsigned __int8 found_specific = 0;
    int participant_better_rating_mask = 0;   /* per-participant "would-be-better-with-player" bits */
    int placed_secondary = 0;
    int can_begin = 1;
    char keep_trying = 0;

    conversation->participant_bitmask = 0;
    /* recovered: (_DWORD*)conversation+9 stride store -> actor_indices[8] = -1 */
    for ( int i = 0; i < 8; ++i )
        conversation->actor_indices[i] = -1;
    /* recovered: (_DWORD*)conversation+6..9 stores -> dialogue_indices[8] = -1 (0xFFFF) */
    for ( int i = 0; i < 8; ++i )
        conversation->dialogue_indices[i] = -1;

    int participant_index = 0;
    int participant_count = definition->participants.count;

    /* Primary pass: place participants that are not secondary (flag 4 clear). */
    if ( participant_count > 0 )
    {
        for ( int slot = 0; participant_index < participant_count; slot = participant_index )
        {
            ai_conversation_participant *participant =
                &((ai_conversation_participant *)definition->participants.address)[slot];
            if ( (participant->flags & (1u << _ai_conversation_participant_is_alternate_bit)) == 0 )
            {
                unsigned __int8 better_rating = 0;
                ai_conversation_find_participant(conversation_index, participant_index,
                                                 &found_specific, &try_alternate, &better_rating, &best_distance);
                if ( better_rating )
                    participant_better_rating_mask |= 1 << slot;
                else
                    participant_better_rating_mask &= ~(1 << slot);
            }
            participant_count = definition->participants.count;
            participant_index = (__int16)(slot + 1);
        }
    }

    unsigned __int8 any_found_specific = found_specific;
    int primary_try_alternate = try_alternate;

    /* Alternate pass: place secondary participants (flag 4 set) not yet filled. */
    if ( try_alternate && participant_count > 0 )
    {
        int slot = 0;
        do
        {
            ai_conversation_participant *participant =
                &((ai_conversation_participant *)definition->participants.address)[slot];
            if ( (conversation->participant_bitmask & (1 << slot)) == 0 && (participant->flags & (1u << _ai_conversation_participant_is_alternate_bit)) != 0 )
            {
                try_alternate = 0;
                if ( ai_conversation_find_participant(conversation_index, slot, &found_specific,
                                                      nullptr, &try_alternate, &best_distance) )
                    placed_secondary = 1;
                else if ( try_alternate )
                    participant_better_rating_mask |= 1 << slot;
                else
                    participant_better_rating_mask &= ~(1 << slot);
            }
            participant_count = definition->participants.count;
            slot = (__int16)(slot + 1);
        }
        while ( slot < participant_count );
        any_found_specific = found_specific;
    }

    /* Find the first participant that blocks the conversation from starting. */
    char blocking_has_better_rating = 0;
    char blocking_hard_miss = 0;
    if ( participant_count > 0 )
    {
        int slot = 0;
        while ( 1 )
        {
            ai_conversation_participant *participant =
                &((ai_conversation_participant *)definition->participants.address)[slot];
            unsigned __int16 flags = participant->flags;
            if ( (flags & (1u << _ai_conversation_participant_optional_bit)) == 0   /* required */
              && (conversation->participant_bitmask & (1 << slot)) == 0             /* not placed */
              && ((flags & (1u << _ai_conversation_participant_has_alternate_bit)) == 0 || !placed_secondary)
              && ((flags & (1u << _ai_conversation_participant_is_alternate_bit)) == 0 || primary_try_alternate) )
                break;
            slot = (__int16)(slot + 1);
            if ( slot >= participant_count )
                goto blocking_resolved;
        }
        if ( ((1 << slot) & participant_better_rating_mask) != 0 )
            blocking_has_better_rating = 1;
        else
            blocking_hard_miss = 1;
    }
blocking_resolved:
    if ( blocking_hard_miss )
    {
        can_begin = 0;
    }
    else if ( blocking_has_better_rating )
    {
        keep_trying = 1;
        can_begin = 0;
    }

    /* Distance gate (flag 0x40): the closest specific participant must be within trigger_dist. */
    if ( can_begin && (definition->flags & (1u << _ai_conversation_keep_trying_to_play_bit)) != 0 )
    {
        float trigger_dist = definition->trigger_dist;
        if ( trigger_dist > 0.0f && any_found_specific && best_distance > trigger_dist )
        {
            can_begin = 0;
            keep_trying = 1;
        }
    }

    conversation->triggering_player_unit_index = -1;

    /* Flag 0x10: pick the triggering player as the one with a participant in attention range. */
    if ( can_begin && (definition->flags & (1u << _ai_conversation_player_must_be_visible_bit)) != 0 )
    {
        if ( any_found_specific )
        {
            float nearest_player_distance = 3.4028235e38f;
            data_iterator player_iterator;
            data_iterator_new(&player_iterator, player_data);
            for ( player_datum *player = data_iterator_next(&player_iterator);
                  player;
                  player = data_iterator_next(&player_iterator) )
            {
                int player_unit_index = player->unit_index;
                if ( player_unit_index == -1 )
                    continue;
                float nearest_prop_distance = 3.4028235e38f;
                if ( definition->participants.count > 0 )
                {
                    for ( int slot = 0; slot < definition->participants.count; slot = (__int16)(slot + 1) )
                    {
                        int actor_index = conversation->actor_indices[slot];
                        if ( actor_index == -1 )
                            continue;
                        int prop_index = prop_get_active_by_unit_index(actor_index, player_unit_index);
                        if ( prop_index != -1 )
                        {
                            prop_datum *prop = DATUM_GET(prop_data, prop_datum, prop_index);
                            __int16 prop_status = prop->state;
                            if ( prop_status >= _prop_state_becoming_unacknowledged && prop_status <= _prop_state_acknowledged && nearest_prop_distance > prop->distance )
                                nearest_prop_distance = prop->distance;
                        }
                    }
                }
                if ( nearest_prop_distance < nearest_player_distance )
                {
                    nearest_player_distance = nearest_prop_distance;
                    conversation->triggering_player_unit_index = player_unit_index;
                }
            }
            if ( conversation->triggering_player_unit_index == -1 )
            {
                can_begin = 0;
                if ( (definition->flags & (1u << _ai_conversation_keep_trying_to_play_bit)) != 0 )
                    keep_trying = 1;
            }
        }
        else
        {
            can_begin = 0;
        }
    }

    /* Flag 0x80: a player must have line-of-sight to at least one placed participant. */
    if ( can_begin && (definition->flags & (1u << _ai_conversation_player_must_be_looking_at_bit)) != 0 && any_found_specific )
    {
        char player_can_see = 0;
        data_iterator player_iterator;
        data_iterator_new(&player_iterator, player_data);
        for ( player_datum *player = data_iterator_next(&player_iterator);
              player;
              player = data_iterator_next(&player_iterator) )
        {
            if ( player_can_see )
                break;
            int player_unit_index = player->unit_index;
            if ( player_unit_index != -1 && definition->participants.count > 0 )
            {
                int slot = 0;
                while ( conversation->actor_indices[slot] == -1
                     || !unit_can_see_point(player_unit_index,
                                            &DATA_ARRAY_ELEMENT(actor_data, actor_datum,
                                                                conversation->actor_indices[slot])->input.position.head_position,
                                            0.52359879f) )
                {
                    slot = (__int16)(slot + 1);
                    if ( slot >= definition->participants.count )
                        goto next_player;
                }
                player_can_see = 1;
            }
        next_player:
            ;
        }
        if ( !player_can_see )
        {
            can_begin = 0;
            if ( (definition->flags & (1u << _ai_conversation_keep_trying_to_play_bit)) != 0 )
                keep_trying = 1;
        }
    }

    if ( can_begin )
    {
        if ( definition->participants.count > 0 )
        {
            int slot = 0;
            do
            {
                if ( ((1 << slot) & conversation->participant_bitmask) != 0
                  && conversation->actor_indices[slot] != -1 )
                {
                    ai_conversation_participant *participant =
                        &((ai_conversation_participant *)definition->participants.address)[slot];
                    int unit_index = DATUM_GET(actor_data, actor_datum,
                                       conversation->actor_indices[slot])->meta.unit_index;
                    __int16 attach_name = participant->new_attach_object_name_index;
                    unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
                    if ( (unsigned __int16)attach_name != 0xFFFF )
                        object_set_object_index_for_name_index(attach_name, unit_index);

                    action_state_data converse_state;
                    if ( (definition->flags & (1u << _ai_conversation_stop_other_actions_bit)) != 0
                      && action_converse_setup(conversation->actor_indices[slot], conversation_index,
                                               &converse_state.___u0.converse) )
                    {
                        actor_action_change(conversation->actor_indices[slot], actor_action_converse, &converse_state);
                    }

                    __int16 chosen_variant = participant->dialogue_variants[conversation->dialogue_indices[slot]];
                    if ( unit_object->object.variant_number != chosen_variant )
                    {
                        int unit_flags = unit_object->unit.flags;
                        unit_object->object.variant_number = chosen_variant;
                        unit_object->unit.flags = unit_flags & ~(1u << _unit_must_set_up_dialogue_bit);
                    }
                }
                slot = (__int16)(slot + 1);
            }
            while ( slot < definition->participants.count );
        }
        conversation->begun = 1;
    }
    else
    {
        *continue_trying = keep_trying && (definition->flags & (1u << _ai_conversation_keep_trying_to_play_bit)) != 0;
    }

    return can_begin;
}
