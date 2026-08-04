/* actor_perception_find_prop_pathfinding_location @0x837D67D4 — lazily resolve and cache the
 * pathfinding surface a prop sits on (prop dword +236). If the prop is on a vehicle (dword +272 != -1)
 * use the vehicle's surface lookup; otherwise, if its unit (dword +24) is a valid object, use the biped
 * surface lookup at the prop's position (real_point3d at prop +80). Prop records are stride 312. */

#include <stdint.h>
#include "headers/prop_datum.h"
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int biped_find_pathfinding_surface_index(int biped_index, real_point3d *pathfinding_point);
extern int vehicle_find_pathfinding_surface_index(int vehicle_index, real_point3d *pathfinding_point);

void actor_perception_find_prop_pathfinding_location(uint16_t actor_index, uint16_t prop_index)
{
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
    if ( prop->pathfinding_surface_index == -1 )
    {
        int vehicle_index = prop->vehicle_index;
        int pathfinding_surface_index;
        if ( vehicle_index == -1 )
        {
            if ( !object_try_and_get_and_verify_type(prop->unit_index, object_mask_biped) )
                return;
            pathfinding_surface_index = biped_find_pathfinding_surface_index(prop->unit_index, &prop->pathfinding_point);
        }
        else
        {
            pathfinding_surface_index = vehicle_find_pathfinding_surface_index(vehicle_index, &prop->pathfinding_point);
        }
        prop->pathfinding_surface_index = pathfinding_surface_index;
    }
}
