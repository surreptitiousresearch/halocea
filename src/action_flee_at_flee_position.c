/* action_flee_at_flee_position @0x838251F8 — check whether an actor has arrived at its assigned flee
 * firing position and can stop there. Requires the actor to be in a real encounter with a valid assigned
 * flee firing position. If the actor's path is already complete and its "confirmed" firing-position slot
 * (+1136) still matches the assigned one (+952) with state 3, it's immediately considered arrived.
 * Otherwise, checks whether the actor's body position is within its destination tolerance (squared) of
 * the firing position's point; if not, not arrived. If it is, also requires (when the actor has a
 * tracked prop) that prop's line-of-sight state be "clear" (0 or 1) before confirming arrival.
 *
 * NOTE: +952 is firing_positions.current_position_index; +1132/+1136 are the destination_orders union
 * (both now resolved via typed members). +52 (meta.encounter_index), +300..308
 * (input.position.body_position) and the ai_encounters firing-position lookup (176-byte stride, offset
 * 156, 24-byte-stride entries) reuse fields already established in
 * action_flee_current_position_exposed.c; +184 (prop_index) and prop->line_of_sight (312-byte stride,
 * offset 56) reuse prop_datum.h's already-resolved layout. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/scenario.h"
#include "headers/firing_position_definition.h"
#include "headers/encounter_definition.h"
#include "headers/destination_type.h"
#include "headers/ai_line_of_sight.h"
#include "headers/blam_data_globals.h"


extern uint8_t actor_path_at_destination(uint16_t actor_index);
extern float actor_destination_tolerance(uint16_t actor_index);

uint8_t action_flee_at_flee_position(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->meta.encounter_index == -1 )
        return 0;

    /* recovered: raw word +952 -> firing_positions.current_position_index (assigned flee firing-position index). */
    int16_t flee_firing_position_index = actor->firing_positions.current_position_index;
    if ( flee_firing_position_index == -1 )
        return 0;

    /* recovered: the folded 176 was sizeof(encounter_definition) and +156 is firing_positions.address
     * (the tag_block starts at 0x98, its address field 4 bytes in). */
    const encounter_definition *encounters =
        (const encounter_definition *)global_scenario->ai_encounters.address;
    const firing_position_definition *firing_positions =
        (const firing_position_definition *)
            encounters[(uint16_t)actor->meta.encounter_index].firing_positions.address;
    const firing_position_definition *firing_position = &firing_positions[flee_firing_position_index];

    /* recovered: 1132 -> control.path.destination_orders.destination_type, 1136 -> its firing_position_index (union @0x4) */
    if ( actor_path_at_destination(actor_index)
      && actor->control.path.destination_orders.destination_type == _destination_firing_position
      && (uint16_t)actor->control.path.destination_orders.___u3.firing_position_index == (uint16_t)actor->firing_positions.current_position_index )
        return 1;

    float tolerance = actor_destination_tolerance(actor_index);

    float dx = firing_position->position.n[0] - actor->input.position.body_position.n[0];
    float dy = firing_position->position.n[1] - actor->input.position.body_position.n[1];
    float dz = firing_position->position.n[2] - actor->input.position.body_position.n[2];

    if ( dx * dx + dy * dy + dz * dz >= tolerance * tolerance )
        return 0;

    uint8_t line_of_sight_blocked = 0;

    /* actor+184 = state.action_data.___u0.flee.flee_prop_index (flee_state_data+28). */
    int prop_index = actor->state.action_data.___u0.flee.flee_prop_index;
    if ( prop_index != -1 )
    {
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
        int16_t line_of_sight = prop->line_of_sight;

        line_of_sight_blocked = (line_of_sight != _ai_line_of_sight_clear && line_of_sight != _ai_line_of_sight_occluded);
    }

    return line_of_sight_blocked == 0;
}
