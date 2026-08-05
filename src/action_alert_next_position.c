/* action_alert_next_position @0x83826788 — pick the next alert firing position for an actor to move to,
 * within its encounter squad's firing-position list. Builds an "unavailable" bit vector over the squad's
 * positions (a position is unavailable if it is the current one, is within 0.5 world units of the actor,
 * fails the position's team filter, or is occupied by a nearby prop in state 2-3), then selects a target:
 *   - order 5: a weighted-random available position (choose_random_array_element);
 *   - orders 2/3/4: walk from the current index in a direction (2 = forward, 3 = ping-pong bouncing off the
 *     ends and otherwise keeping the prior direction, 4 = random per game tick) to the next available slot.
 * Returns the chosen position index, or -1 if the actor is disabled, has no order, no encounter, or no
 * position is available.
 *
 * Sibling of action_alert_perform.c (its caller). Actor typed actor_datum* for meta.encounter_index /
 * meta.squad_index; the scenario squad / move-position records are typed encounter_definition /
 * squad_definition / move_position_definition (80-byte stride: position xyz at 0/4/8, weight at 16,
 * sequence_id team-filter byte at 30). Distance tests use squared distance < 0.25 (0.5^2). */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/scenario.h"
#include "headers/prop_iterator.h"
#include "headers/prop_datum.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/move_position_definition.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern int choose_random_array_element(char *array, int16_t element_size, int16_t element_count, int16_t weight_field_offset, unsigned int *used_bit_vector);
extern int game_time_get(void);

int16_t action_alert_next_position(int actor_index, int16_t move_position_order, int16_t current_position_index, uint8_t *direction_increasing)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->input.vehicle_passenger )   /* actor disabled/blind */
        return -1;
    if ( !move_position_order )
        return -1;
    if ( actor->meta.encounter_index == -1 )
        return -1;

    encounter_definition *encounter =
        (encounter_definition *)global_scenario->ai_encounters.address + (uint16_t)actor->meta.encounter_index;
    squad_definition *squad = (squad_definition *)encounter->squads.address + actor->meta.squad_index;

    if ( move_position_order == 1 && current_position_index != -1 )
        return current_position_index;

    int position_count = squad->move_positions.count;
    char any_available = 0;
    /* Only word 0 is initialized; a squad never has more than 32 firing positions (faithful — the disasm
     * zeroes only unavailable_mask[0], and every index/test below stays within word 0). */
    unsigned int unavailable_mask[2];
    unavailable_mask[0] = 0;

    if ( position_count > 0 )
    {
        for ( int i = 0; i < position_count; i = (int16_t)(i + 1) )
        {
            char available = (current_position_index != i);
            move_position_definition *firing_position = (move_position_definition *)squad->move_positions.address + i;

            if ( current_position_index != -1 )
            {
                float dz = firing_position->position.n[2] - actor->input.position.body_position.z;
                float dx = firing_position->position.n[0] - actor->input.position.body_position.x;
                float dy = firing_position->position.n[1] - actor->input.position.body_position.y;
                if ( dy * dy + (dx * dx + dz * dz) < 0.25f )
                    available = 0;
            }

            if ( firing_position->sequence_id )
                available = actor->state.noncombat_sequence_id != firing_position->sequence_id ? 0 : available;

            prop_iterator iterator;
            prop_iterator_new(&iterator, actor_index);
            for ( prop_datum *prop = prop_iterator_next(&iterator); prop; prop = prop_iterator_next(&iterator) )
            {
                int state = prop->state;
                if ( state >= _prop_state_becoming_unacknowledged && state <= _prop_state_acknowledged )
                {
                    float dz = firing_position->position.n[2] - prop->body_position.n[2];
                    float dx = firing_position->position.n[0] - prop->body_position.n[0];
                    float dy = firing_position->position.n[1] - prop->body_position.n[1];
                    if ( dy * dy + (dx * dx + dz * dz) < 0.25f )
                    {
                        available = 0;
                        break;
                    }
                }
            }

            if ( available )
                any_available = 1;
            else
                BIT_VECTOR_SET_FLAG(unavailable_mask, i);

            position_count = squad->move_positions.count;
        }
    }

    if ( any_available )
    {
        if ( move_position_order == 5 )
            return choose_random_array_element(squad->move_positions.address, 80, position_count, 16, unavailable_mask);

        int16_t index;
        if ( current_position_index < 0 || current_position_index >= position_count )
            index = 0;
        else
            index = current_position_index;

        while ( 1 )
        {
            uint8_t increasing;
            if ( move_position_order == 3 && index != 0 )
            {
                if ( index == position_count - 1 )
                {
                    increasing = 0;
                }
                else
                {
                    increasing = 1;
                    if ( direction_increasing )
                        increasing = *direction_increasing;
                }
            }
            else if ( move_position_order == 4 )
            {
                increasing = game_time_get() & 1;
            }
            else
            {
                increasing = 1;
            }

            if ( direction_increasing )
                *direction_increasing = increasing;

            if ( increasing )
            {
                index = (int16_t)(index + 1);
                if ( index >= position_count )
                    index = 0;
            }
            else
            {
                int16_t previous = index - 1;
                index = (int16_t)(index - 1);
                if ( previous < 0 )
                    index = (int16_t)(position_count - 1);
            }

            if ( !BIT_VECTOR_TEST_FLAG(unavailable_mask, index) )
                return index;
        }
    }

    return -1;
}
