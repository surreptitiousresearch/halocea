/* ai_conversation_find_participant @0x837CD208 — choose the best unit to fill one participant slot of a
 * running conversation, and bind it into the conversation datum.
 *
 * Selection proceeds by the participant's selection_type and a candidate source: an explicit scenario
 * object (preexisting_object_name_index), an AI-index actor set (runtime_ai_index), or all active actors.
 * Each candidate of the required actor_type that is not already used by another slot is scored: a player
 * rating (line-of-sight / proximity), a closeness bonus relative to the already-placed participants, a
 * vehicle/leadership/enmity adjustment per selection_type, and a +0.7 bonus when a dialogue variant
 * matching the unit's current variant is available. The highest-scoring candidate is recorded.
 *
 * Returns 1 (and fills found_specific_unit_reference / writes the chosen actor + dialogue line into the
 * conversation) when a participant was placed; 0 otherwise. The optional out-references report: a specific
 * unit reference was required but unresolved (try_alternate_reference), a candidate would have qualified
 * but the player rating was too low (success_with_better_player_rating_reference), and the smallest player
 * distance seen (best_distance_reference). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/conversation_participant_definition_flags.h"
#include "headers/scenario.h"
#include "headers/conversation_datum.h"
#include "headers/ai_conversation.h"
#include "headers/ai_conversation_participant.h"
#include "headers/actor_datum.h"
#include "headers/actor_iterator.h"
#include "headers/ai_index_actor_iterator.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/ai_conversation_selection_type.h"
#include "headers/blam_data_globals.h"


extern int object_index_from_name_index(int16_t name_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void actor_iterator_new(actor_iterator *iterator, uint8_t active_only);
extern actor_datum *actor_iterator_next(actor_iterator *iterator);
extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern float ai_communication_get_player_rating(int unit_index, uint8_t test_line_of_sight, int *unit_index_reference, float *distance_reference);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern float __fsqrts(float x);

uint8_t ai_conversation_find_participant(
        int conversation_index,
        int16_t participant_index,
        uint8_t *found_specific_unit_reference,
        uint8_t *try_alternate_reference,
        uint8_t *success_with_better_player_rating_reference,
        float *best_distance_reference)
{
    conversation_datum *conversation =
        DATA_ARRAY_ELEMENT(conversation_data, conversation_datum, conversation_index);
    ai_conversation *definition =
        &((ai_conversation *)global_scenario->ai_conversations.address)[conversation->conversation_definition_index];

    float best_distance = 3.4028235e38f;
    int assigned = 0;
    char better_player_rating_seen = 0;
    int assigned_actor_index = -1;
    int16_t assigned_line_index = -1;

    ai_conversation_participant *participant =
        &((ai_conversation_participant *)definition->participants.address)[participant_index];
    int16_t selection_type = participant->selection_type;

    if ( selection_type == _ai_conversation_selection_disembodied )
    {
        /* disembodied — no actor required: always satisfied with line 0. */
        assigned_line_index = 0;
        assigned = 1;
        goto finalize;
    }

    {
        char is_player_selection = 0;          /* radio / radio_sargeant */
        char using_ai_index_iterator = 0;
        uint8_t use_object_directly = 0;
        int candidate_object_index = -1;
        float best_score = 0.0f;

        if ( selection_type == _ai_conversation_selection_radio
          || selection_type == _ai_conversation_selection_radio_sargeant )
            is_player_selection = 1;

        int participant_count = definition->participants.count;

        /* Cache the body positions of the participants already placed, for the closeness bonus.
         * (DEVIATION: the decompiler aliases this scratch onto the unused tail of the actor_iterator
         *  buffer; reproduced here as a dedicated array of the same 12-byte stride.) */
        float placed_positions[8][3];
        int placed_count = 0;
        for ( int slot = 0; slot < participant_count; slot = (int16_t)(slot + 1) )
        {
            if ( conversation->actor_indices[slot] != -1 )
            {
                actor_datum *placed =
                    DATA_ARRAY_ELEMENT(actor_data, actor_datum, conversation->actor_indices[slot]);
                placed_positions[placed_count][0] = placed->input.position.body_position.n[0];
                placed_positions[placed_count][1] = placed->input.position.body_position.n[1];
                placed_positions[placed_count][2] = placed->input.position.body_position.n[2];
                ++placed_count;
            }
        }

        uint8_t first_participant = placed_count == 0;

        actor_iterator all_actor_iterator;
        ai_index_actor_iterator ai_actor_iterator;

        int16_t preexisting_name = participant->preexisting_object_name_index;
        if ( (uint16_t)preexisting_name == 0xFFFF )
        {
            int runtime_ai_index = participant->runtime_ai_index;
            if ( runtime_ai_index == -1 )
            {
                actor_iterator_new(&all_actor_iterator, 1u);
            }
            else
            {
                ai_index_actor_iterator_new(runtime_ai_index, &ai_actor_iterator);
                using_ai_index_iterator = 1;
            }
        }
        else
        {
            candidate_object_index = object_index_from_name_index(preexisting_name);
            use_object_directly = 1;
        }

        for ( ;; )
        {
            float candidate_distance = 3.4028235e38f;
            float candidate_score = 0.0f;
            object_datum *candidate_target_object = nullptr;
            actor_datum *candidate;
            int candidate_index;

            if ( use_object_directly )
            {
                unit_datum *object = object_try_and_get_and_verify_type(candidate_object_index, object_mask_unit);
                candidate = nullptr;
                candidate_index = -1;
                if ( object && object->unit.actor_index != -1 )
                {
                    candidate_index = object->unit.actor_index;
                    candidate = DATA_ARRAY_ELEMENT(actor_data, actor_datum, candidate_index);
                }
                candidate_object_index = -1;   /* only the single named object is tried */
            }
            else if ( using_ai_index_iterator )
            {
                candidate = ai_index_actor_iterator_next(&ai_actor_iterator);
                candidate_index = ai_actor_iterator.iterator.index;
            }
            else
            {
                candidate = actor_iterator_next(&all_actor_iterator);
                candidate_index = all_actor_iterator.index;
            }

            if ( !candidate )
                goto finalize;

            int candidate_unit_index = candidate->meta.unit_index;
            if ( candidate_unit_index == -1
              || (uint16_t)candidate->meta.type != (uint16_t)participant->actor_type )
                continue;

            /* Skip if this candidate is already bound to another participant slot. */
            int placed_slot = 0;
            if ( participant_count > 0 )
            {
                for ( placed_slot = 0; placed_slot < participant_count; placed_slot = (int16_t)(placed_slot + 1) )
                {
                    if ( candidate_index == conversation->actor_indices[placed_slot] )
                        break;
                }
            }
            if ( (int16_t)placed_slot < participant_count )
                continue;

            int rating_target_unit_index;
            float player_rating = ai_communication_get_player_rating(candidate_unit_index, first_participant,
                                                                     &rating_target_unit_index, &candidate_distance);
            if ( rating_target_unit_index == -1 )
            {
                if ( !is_player_selection )
                {
                    better_player_rating_seen = 1;
                    continue;
                }
            }
            else
            {
                candidate_score = player_rating;
                candidate_target_object =
                    DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, rating_target_unit_index)->datum;
            }

            unsigned int valid = 1;
            unsigned int selection = (uint16_t)participant->selection_type;
            if ( selection > _ai_conversation_selection_radio_sargeant || selection == _ai_conversation_selection_disembodied )
                goto evaluate_dialogue;
            if ( selection == _ai_conversation_selection_in_player_vehicle )
                goto same_vehicle;
            if ( selection == _ai_conversation_selection_not_in_vehicle && participant->selection_type )
            {
                valid = candidate->input.vehicle_index == -1;   /* require not riding a vehicle */
                goto evaluate_dialogue;
            }
            if ( selection == _ai_conversation_selection_sargeant && participant->selection_type )
                goto leader_bonus;
            if ( selection != _ai_conversation_selection_any_actor || !participant->selection_type )
            {
                if ( (selection != _ai_conversation_selection_radio || !participant->selection_type) && participant->selection_type )
                {
                leader_bonus:
                    if ( candidate->meta.unique_leader )
                        candidate_score = candidate_score + 1.5f;
                    goto evaluate_dialogue;
                }
                if ( candidate_target_object
                  && game_team_is_enemy(candidate->meta.team_index, candidate_target_object->object.owner_team_index) )
                    goto invalid;   /* reject enemy of the player target */
            }
            goto evaluate_dialogue;

        same_vehicle:
            if ( !candidate_target_object )
            {
                valid = 0;
                goto evaluate_dialogue;
            }
            {
                int target_vehicle_index = candidate_target_object->object.parent_object_index;
                if ( target_vehicle_index != -1 && candidate->input.vehicle_index == target_vehicle_index )
                {
                    if ( candidate->input.vehicle_gunner )
                        candidate_score = candidate_score + 1.0f;
                    goto evaluate_dialogue;
                }
            }
        invalid:
            valid = 0;

        evaluate_dialogue:
            if ( !valid )
                continue;

            if ( first_participant && !is_player_selection && player_rating < 2.0f
              && definition->run_to_player_dist == 0.0f )
            {
                better_player_rating_seen = 1;
                continue;
            }

            /* Closeness bonus: reward candidates near the already-placed participants. */
            if ( placed_count > 0 )
            {
                float nearest = 3.4028235e38f;
                for ( int i = 0; i < placed_count; i = (int16_t)(i + 1) )
                {
                    float dx = placed_positions[i][0] - candidate->input.position.body_position.n[0];
                    float dy = placed_positions[i][1] - candidate->input.position.body_position.n[1];
                    float dz = placed_positions[i][2] - candidate->input.position.body_position.n[2];
                    float distance_squared = dx * dx + (dy * dy + dz * dz);
                    if ( nearest > distance_squared )
                        nearest = distance_squared;
                }
                if ( nearest < 20.25f )
                    candidate_score = candidate_score
                                    - (((__fsqrts(nearest) - 1.5f) * 0.33333334f) - 1.0f);
            }

            /* Pick a dialogue variant: prefer one matching the unit's current variant (or the empty
             * variant), otherwise choose at random from the matching set. */
            int16_t matching_variants[8];
            int16_t matching_count = 0;
            int16_t exact_variant_slot = -1;
            char have_exact_variant = 0;
            int16_t candidate_current_variant =
                (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, candidate->meta.unit_index)->datum)->object.variant_number;

            int variant_slot = 0;
            for ( ;; )
            {
                int16_t variant = participant->dialogue_variants[variant_slot];
                if ( variant != -1 )
                {
                    if ( variant == candidate_current_variant )
                    {
                        exact_variant_slot = variant_slot;
                        have_exact_variant = 1;
                        break;
                    }
                    if ( variant )
                    {
                        if ( candidate_current_variant < 100 && variant < 100 )
                            matching_variants[matching_count++] = variant_slot;
                    }
                    else
                    {
                        exact_variant_slot = variant_slot;
                        have_exact_variant = 1;
                    }
                }
                variant_slot = (int16_t)(variant_slot + 1);
                if ( variant_slot >= 6 )
                    break;
            }

            int16_t chosen_variant_slot;
            if ( have_exact_variant )
            {
                candidate_score = candidate_score + 0.7f;
                chosen_variant_slot = exact_variant_slot;
            }
            else if ( matching_count > 0 )
            {
                if ( matching_count == 1 )
                {
                    chosen_variant_slot = matching_variants[0];
                }
                else
                {
                    uint32_t *seed = get_global_random_seed_address();
                    chosen_variant_slot = matching_variants[seed_random_range(seed, 0, matching_count)];
                }
            }
            else
            {
                continue;   /* no usable dialogue variant — reject candidate */
            }

            if ( candidate_score > best_score )
            {
                best_distance = candidate_distance;
                assigned_actor_index = candidate_index;
                best_score = candidate_score;
                assigned_line_index = chosen_variant_slot;
                assigned = 1;
            }
        }
    }

finalize:
    if ( (unsigned char)assigned )
    {
        conversation->participant_bitmask |= 1u << participant_index;
        conversation->actor_indices[participant_index] = assigned_actor_index;
        conversation->dialogue_indices[participant_index] = assigned_line_index;
        if ( found_specific_unit_reference && assigned_actor_index != -1 )
            *found_specific_unit_reference = 1;
    }
    else if ( (participant->flags & (1u << _ai_conversation_participant_has_alternate_bit)) != 0 && try_alternate_reference )
    {
        *try_alternate_reference = 1;
    }

    if ( better_player_rating_seen && success_with_better_player_rating_reference )
        *success_with_better_player_rating_reference = 1;
    if ( best_distance_reference && *best_distance_reference > best_distance )
        *best_distance_reference = best_distance;
    return assigned;
}
