/* actor_find_pathfinding_location @0x8371ECF0 — caches an actor's pathfinding surface index once per
 * "location" (only recomputed while `pathfinding_surface_index` is still -1): first snapshots the
 * actor's current body position into its pathfinding point, then — unless the actor is flying — looks
 * up the surface under it via `biped_find_pathfinding_surface_index` (no unit attached) or
 * `vehicle_find_pathfinding_surface_index` (attached to a 2/3 -type vehicle seat), storing the result.
 * Clean decompile, matches disasm/applied prototype directly; the `(real_point3d*)actor + 30` pointer
 * arithmetic is the same actor+0x168 (`input.pathfinding_point`) idiom already established in
 * `actor_path_input_new.c`. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/real_point3d.h"
#include "headers/object_type.h"
#include "headers/actor_vehicle_driver_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int biped_find_pathfinding_surface_index(int biped_index, real_point3d *pathfinding_point);
extern int vehicle_find_pathfinding_surface_index(int vehicle_index, real_point3d *pathfinding_point);

void actor_find_pathfinding_location(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if (actor->input.pathfinding_surface_index != -1)
        return;

    actor->input.pathfinding_point = actor->input.position.body_position;

    if (actor->state.flying)
        return;

    int vehicle_index = actor->input.vehicle_index;
    if (vehicle_index == -1)
    {
        if (!object_try_and_get_and_verify_type(actor->meta.unit_index, object_mask_biped))
            return;
        actor->input.pathfinding_surface_index =
                biped_find_pathfinding_surface_index(actor->meta.unit_index, &actor->input.pathfinding_point);
        return;
    }

    unsigned __int16 vehicle_driver_type = (unsigned __int16)actor->input.vehicle_driver_type;
    if (vehicle_driver_type >= _actor_vehicle_driver_hovering_ground
        && vehicle_driver_type <= _actor_vehicle_driver_nondirectional_ground)
    {
        actor->input.pathfinding_surface_index =
                vehicle_find_pathfinding_surface_index(vehicle_index, &actor->input.pathfinding_point);
    }
}
