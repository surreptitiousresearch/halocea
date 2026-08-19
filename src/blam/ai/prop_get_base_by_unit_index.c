/* prop_get_base_by_unit_index @0x837D1FE0 — find the actor's prop (its working knowledge entry) for a given
 * unit, optionally creating one. Walks the actor's prop list (meta.first_prop_index -> prop.next_prop_index)
 * for a prop whose unit matches, or — for swarm props — whose actor matches the unit's controlling actor. A
 * matched prop's orphan (prop.orphan_prop_index) is preferred when present. If none is found and `acquire` is
 * set (and the actor is active), a new unacknowledged prop is created, position-refreshed, and — when
 * `refresh_upon_acquisition` is set — status-refreshed and (if sufficiently perceived) acknowledged.
 *
 * Returns the prop index, or -1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/prop_datum.h"
#include "headers/actor_position_data.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern int prop_new_unacknowledged(int actor_index, int unit_index, uint8_t enemy);
extern void prop_position_refresh(int actor_index, int prop_index, actor_position_data *sense_position, uint8_t forced_position_update, uint8_t full_status_update);
extern void prop_status_refresh(int actor_index, int prop_index, actor_position_data *sense_position);
extern int actor_expected_acknowledgement(int actor_index, int prop_index);
extern void actor_perception_acknowledge(int actor_index, int prop_index, uint8_t had_orphan, uint8_t expected);

int prop_get_base_by_unit_index(int actor_index, int unit_index, uint8_t acquire,
                                uint8_t refresh_upon_acquisition)
{
    int prop_index = -1;
    if ( unit_index == -1 )
        return -1;

    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    int unit_actor_index = unit->unit.swarm_actor_index;
    if ( unit_actor_index == -1 )
        unit_actor_index = unit->unit.actor_index;

    if ( unit->object.type || unit_actor_index == actor_index )
        return -1;

    prop_datum *prop = 0;
    int matched_prop_index = -1;
    int next_index = actor->meta.first_prop_index;
    while ( 1 )
    {
        matched_prop_index = next_index;
        if ( next_index == -1 )
            goto acquire_path;

        prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, next_index);
        next_index = prop->next_prop_index;
        if ( prop->unit_index == unit_index )
            break;
        if ( prop->swarm && prop->actor_index != -1 && prop->actor_index == unit_actor_index )
            break;
    }

    prop_index = (prop->___u3.orphan_prop_index != -1) ? prop->___u3.orphan_prop_index : matched_prop_index;
    if ( prop_index != -1 )
        return prop_index;

acquire_path:
    if ( acquire && actor->meta.active )
    {
        uint8_t enemy = game_team_is_enemy(actor->meta.team_index, unit->object.owner_team_index);
        prop_index = prop_new_unacknowledged(actor_index, unit_index, enemy);
        if ( prop_index != -1 )
        {
            prop_datum *new_prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
            actor_position_data sense_position;
            prop_position_refresh(actor_index, prop_index, &sense_position, 0, refresh_upon_acquisition);
            new_prop->required_ticks = 30;
            new_prop->delay_requirement_decision = 1;
            if ( refresh_upon_acquisition )
            {
                prop_status_refresh(actor_index, prop_index, &sense_position);
                if ( new_prop->perception >= 2 )
                {
                    uint8_t expected = actor_expected_acknowledgement(actor_index, prop_index);
                    new_prop->state = _prop_state_acknowledged;
                    actor_perception_acknowledge(actor_index, prop_index, 0, expected);
                }
            }
        }
    }
    return prop_index;
}
