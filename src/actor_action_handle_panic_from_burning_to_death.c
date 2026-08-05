/* actor_action_handle_panic_from_burning_to_death @0x837F1E70 — start (or extend) a panic when the actor
 * is burning to death (flag byte +437): resolve the burn's responsible unit (from the actor's own unit's
 * recent-damage object dword +1040, following vehicles/parents) to a prop as the panic target, latching it
 * (dword +780) only when no panic is in progress or no prop is latched, and raise panic_type
 * (+776) to at least _actor_panic_burning_to_death. Returns 1 when a panic was triggered. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/actor_panic_type.h"
#include "headers/blam_data_globals.h"


extern int ai_get_responsible_unit(int object_index, uint8_t responsible_for_weapon_fire);
extern int prop_get_active_by_unit_index(uint16_t actor_index, int unit_index);

uint8_t actor_action_handle_panic_from_burning_to_death(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( !actor->input.burning_to_death )
        return 0;

    int panic_prop_index = -1;
    int unit_index = actor->meta.unit_index;
    if ( unit_index != -1 )
    {
        unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
        int responsible_unit = ai_get_responsible_unit(unit->unit.flaming_death_attacker_object_index, 1u);
        if ( responsible_unit != -1 )
            panic_prop_index = prop_get_active_by_unit_index(actor_index, responsible_unit);
    }

    /* floor is a panic_type priority, not a tick count */
    int16_t panic_type = actor->stimuli.panic_type;
    if ( !actor->stimuli.panic_type || actor->stimuli.panic_prop_index == -1 )
        actor->stimuli.panic_prop_index = panic_prop_index;
    if ( panic_type <= _actor_panic_burning_to_death )
        panic_type = _actor_panic_burning_to_death;
    actor->stimuli.panic_type = panic_type;
    return 1;
}
