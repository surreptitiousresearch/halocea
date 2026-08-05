/* actor_handle_damage @0x8371E378 — record that an actor's unit took damage from an aggressor. Looks up the
 * actor's "prop" memory for the aggressor (prop_get_base_by_unit_index, acquiring/refreshing it), accumulates
 * the damage fraction into the prop's danger accumulator (+112), clears its decay timer (+108) and marks it
 * dirty (+116). The same is applied to the prop's linked/parent prop (+12) if present. Finally a combat
 * stimulus is raised — but only for a prop of a "live threat" kind (type +36 in [2,3]); otherwise -1 is
 * passed so actor_stimulus_damage gets no prop.
 *
 * Deviation: the decompiler reads damage_velocity from r5 (v6) but the disasm saves r6 (the 4th argument)
 * into r30 and forwards it; corrected to the damage_velocity parameter. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/real_vector3d.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"

extern int prop_get_base_by_unit_index(int actor_index, int unit_index, uint8_t acquire, uint8_t refresh_upon_acquisition);
extern void actor_stimulus_damage(int actor_index, int prop_index, float damage_fraction, real_vector3d *damage_velocity);

void actor_handle_damage(int actor_index, int aggressor_unit_index, float damage_fraction,
                         real_vector3d *damage_velocity)
{
    if ( aggressor_unit_index == -1 )
        return;

    int prop_index = prop_get_base_by_unit_index(actor_index, aggressor_unit_index, 1, 1);
    int stimulus_prop_index = prop_index;
    if ( prop_index != -1 )
    {
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
        int linked_prop_index = prop->___u3.parent_prop_index;
        prop->damage_inflicted_on_me += damage_fraction;
        prop->ticks_since_damage = 0;
        prop->currently_damaging_me = 1;

        if ( linked_prop_index != -1 )
        {
            prop_datum *linked_prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, linked_prop_index);
            linked_prop->ticks_since_damage = 0;
            linked_prop->damage_inflicted_on_me += damage_fraction;
            linked_prop->currently_damaging_me = 1;
        }

        int16_t prop_type = prop->state;
        if ( prop_type < _prop_state_becoming_unacknowledged || prop_type > _prop_state_acknowledged )
            stimulus_prop_index = -1;
    }

    actor_stimulus_damage(actor_index, stimulus_prop_index, damage_fraction, damage_velocity);
}
