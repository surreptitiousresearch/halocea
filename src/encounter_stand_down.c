/* encounter_stand_down @0x8370B008 — marks an encounter as stood-down (no longer fighting): sets the
 * encounter's stand-down flag, clears its pursuit, then walks every actor in the encounter and drops
 * each actor's combat props that target an active enemy (prop state 4–5) other than the actor's own
 * current target — orphaning the prop link, switching the actor off it, and deleting it.
 *
 * DEVIATION: when AI is not initialised for the map the decompiler reads an uninitialised local for the
 * starting actor index; the actor loop is itself gated on ai_initialized_for_map so it never runs in
 * that case — reproduced as -1. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/encounter_datum.h"
#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


extern void encounter_clear_pursuit(uint16_t encounter_index);
extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern void actor_switch_props(uint16_t actor_index, int prop_index, int replacement_prop_index);
extern void prop_delete(int actor_index, int prop_index);

void encounter_stand_down(int encounter_index)
{
    encounter_datum *encounter = DATUM_GET(encounter_data, encounter_datum, encounter_index);
    encounter->stand_down = 1;
    encounter->enemies_defeated = 0;
    encounter_clear_pursuit(encounter_index);

    int actor_index;
    if (ai_globals->ai_initialized_for_map)
    {
        if (encounter_index == -1)
            actor_index = ai_globals->first_encounterless_actor_index;
        else
            actor_index = encounter->first_actor_index;
    }
    else
    {
        actor_index = -1;
    }

    while (ai_globals->ai_initialized_for_map)
    {
        int current_actor = actor_index;
        if (actor_index == -1)
            break;

        actor_datum *actor = DATUM_GET(actor_data, actor_datum, actor_index);
        actor_index = actor->meta.next_actor_index;   /* next actor in the encounter */

        prop_iterator iterator;
        prop_iterator_new(&iterator, current_actor);
        for (prop_datum *prop = prop_iterator_next(&iterator); prop; prop = prop_iterator_next(&iterator))
        {
            int state = prop->state;
            if (state >= _prop_state_uninspected_orphan && state <= _prop_state_inspected_orphan && prop->enemy && iterator.index != actor->target.target_prop_index)
            {
                /* recovered: *((int *)prop_data->data + 78*idx + 3) -> prop.orphan_prop_index (union off 0x0C) */
                (DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop->___u3.orphan_prop_index))->___u3.orphan_prop_index = -1;
                actor_switch_props(current_actor, iterator.index, -1);
                prop_delete(current_actor, iterator.index);
            }
        }
    }
}
