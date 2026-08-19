/* actor_perception_tried_to_search @0x837D9B40 — marks a prop's tried_to_search flag when the actor makes a
 * search attempt against it; if the prop is the actor's current target (dword +156, "actor's active prop"
 * per actor_situation_update.c), refreshes the actor's target/combat situation status. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"

extern void actor_situation_update_target_status(int actor_index);
extern void actor_situation_combat_status_update(int actor_index);

void actor_perception_tried_to_search(int actor_index, int prop_index)
{
    if ( prop_index != -1 )
    {
        actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

        DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index)->tried_to_search = 1;

        if ( prop_index == actor->target.target_prop_index )
        {
            actor_situation_update_target_status(actor_index);
            actor_situation_combat_status_update(actor_index);
        }
    }
}
