/* vehicle_find_pathfinding_surface_index @0x8375DA40 — for ground-based vehicle types, casts a short
 * ray straight down from just above the vehicle's origin and returns the collision-BSP surface index
 * it lands on, writing the contact point to pathfinding_point. Returns -1 (and leaves pathfinding_point
 * at the raw origin) for vehicle motion types that don't use a pathfinding surface, or on a miss.
 *
 * DEVIATION: the decompiler mislabels the local collision result buffer as `global_down3d`; the
 * disassembly passes a stack-local collision_bsp_test_vector_result (as in biped_find_ground_surface).
 * The vehicle motion-type field at definition+756 is re-read several times in the gating condition;
 * reproduced via a single motion_type local (the field is unchanged across the reads). */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/vehicle_definition.h"
#include "headers/vehicle_type.h"
#include "headers/collision_bsp_test_vector_result.h"
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_bsp_test_flags.h"
#include "headers/blam_data_globals.h"


extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern uint8_t collision_bsp_test_vector(unsigned int flags, const struct collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *point, const real_vector3d *vector, float maximum_t, collision_bsp_test_vector_result *result);

int vehicle_find_pathfinding_surface_index(int vehicle_index, real_point3d *pathfinding_point)
{
    int surface_index = -1;

    char *object = ((char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
    struct vehicle_definition *definition =
        TAG_GET(struct vehicle_definition, *(int *)object);
    object_get_origin(vehicle_index, pathfinding_point);

    /* motion_type domain: vehicle_definition_type enum ($9579021B0EF87E584E9713AE3E8182DC):
     * 0=human_tank 1=human_jeep 2=human_boat 3=human_plane 4=alien_scout 5=alien_fighter 6=turret.
     * This (faithfully reproduced, redundancy-laden) decompiler boolean passes the ground-based types
     * (tank/jeep/scout/turret) and rejects boat/plane/alien_fighter (water + _vehicle_mask_flying). */
    unsigned int motion_type = (uint16_t)definition->vehicle.type;
    if (motion_type <= _vehicle_type_turret
        && (motion_type == _vehicle_type_human_jeep
            || (motion_type != _vehicle_type_human_boat
                && (motion_type != _vehicle_type_human_plane || !motion_type)
                && ((motion_type == _vehicle_type_alien_scout && motion_type) || motion_type != _vehicle_type_alien_fighter || !motion_type))))
    {
        const collision_bsp *bsp = global_collision_bsp;

        real_point3d point;
        object_get_origin(vehicle_index, &point);
        point.n[0] += global_up3d->n[0] * 0.40000001f;
        point.n[1] += global_up3d->n[1] * 0.40000001f;
        point.n[2] += global_up3d->n[2] * 0.40000001f;

        real_vector3d vector;
        vector.n[0] = global_down3d->n[0] * 2.0f;
        vector.n[1] = global_down3d->n[1] * 2.0f;
        vector.n[2] = global_down3d->n[2] * 2.0f;

        collision_bsp_test_vector_result result;
        if (collision_bsp_test_vector((1u << _collision_bsp_test_front_facing_surfaces_bit), bsp, 0, nullptr, &point, &vector, 3.4028235e38f, &result))
        {
            surface_index = result.surface_index;
            pathfinding_point->n[0] = vector.n[0] * result.t + point.n[0];
            pathfinding_point->n[1] = vector.n[1] * result.t + point.n[1];
            pathfinding_point->n[2] = vector.n[2] * result.t + point.n[2];
        }
    }
    return surface_index;
}
