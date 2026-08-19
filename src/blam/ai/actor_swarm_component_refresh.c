/* actor_swarm_component_refresh @0x8371DAC8 — refresh a swarm component's cached surface_index and position from
 * its unit. The surface index is taken from the unit's biped.support_surface_index (offset 0x4D8) unless the
 * unit is multiplayer-spawned (object.type set), in which case it is left as -1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/swarm_component_datum.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

extern real_point3d *object_get_origin(int object_index, real_point3d *origin);

void actor_swarm_component_refresh(int unit_index, int swarm_component_index)
{
    int spawn_surface_index = -1;
    swarm_component_datum *component =
        DATA_ARRAY_ELEMENT(swarm_component_data, swarm_component_datum, swarm_component_index);
    biped_datum *unit = (biped_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if ( !unit->object.type )
        spawn_surface_index = unit->biped.support_surface_index;
    object_get_origin(unit_index, &component->position);
    component->surface_index = spawn_surface_index;
}
