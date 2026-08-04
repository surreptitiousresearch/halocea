/* actor_swarm_cache_new @0x83720260 — build the swarm cache for a swarm actor: allocate a swarm_data datum, link
 * it to the actor, then for each unit already in the actor's controlled-unit chain allocate a swarm component
 * (recording the unit index, its position and — for non-multiplayer units — the unit's support surface index).
 * Stops early if a component allocation fails. Returns the swarm cache index (-1 on failure). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/swarm_datum.h"
#include "headers/object_datum.h"
#include "headers/biped_datum.h"
#include "headers/swarm_component_datum.h"
#include "headers/unit_datum.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


extern int datum_new(data_array *data);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);

int actor_swarm_cache_new(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->meta.swarm_cache_index == -1 )
    {
        int swarm_index = datum_new(swarm_data);
        actor->meta.swarm_cache_index = swarm_index;
        if ( swarm_index != -1 )
        {
            int unit_index = actor->meta.swarm_unit_index;
            swarm_datum *swarm = DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, swarm_index);
            swarm->actor_index = actor_index;
            for ( swarm->unit_count = 0; unit_index != -1; )
            {
                int unit = (int)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
                int component_index = datum_new(swarm_component_data);
                if ( component_index == -1 )
                    break;
                int spawn_surface_index = -1;
                swarm_datum *swarm_cache =
                    DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
                /* (component_index << 6) & 0x3FFFC0 = 64 * (u16)component_index = the datum element offset */
                swarm_component_datum *component =
                    DATA_ARRAY_ELEMENT(swarm_component_data, swarm_component_datum, component_index);
                component->combat_target_prop_index = -1;
                swarm_cache->unit_indices[swarm_cache->unit_count] = unit_index;
                swarm_cache->component_indices[swarm_cache->unit_count++] = component_index;
                biped_datum *object_data =
                    (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
                if ( !object_data->object.type )  /* not multiplayer-spawned */
                    spawn_surface_index = object_data->biped.support_surface_index;  /* +1240 */
                object_get_origin(unit_index, &component->position);
                component->surface_index = spawn_surface_index;
                unit_index = ((unit_datum *)unit)->unit.swarm_next_unit_index;
            }
        }
    }
    return actor->meta.swarm_cache_index;
}
