/* nearby_vehicle @0x83747FD0 — true if any vehicle object sits within a small sphere around a candidate spawn
 * point. Builds a scenario location from the point, gathers nearby objects, and reports a hit on the first
 * one whose object type is "vehicle" (1). The DB's second prototype argument is a phantom: the sole
 * caller never sets up r4, so the true interface is single-argument. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/object_header_datum.h"
#include "headers/location.h"
#include "headers/scenario_player.h"
#include "headers/data_array.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void scenario_location_from_point(location *location, const real_point3d *point);
extern int16_t objects_in_sphere(unsigned int class_flags, unsigned int type_flags, const location *location, const real_point3d *center, float radius, int *object_indices, int16_t maximum_count);

/* The only parameter is the candidate point (DB mistyped/named it "player_index"). */
uint8_t nearby_vehicle(const real_point3d *center)
{
    location sphere_location[2];
    int object_indices[16];

    scenario_location_from_point(sphere_location, center);

    /* radius 0.1; object_indices buffer and count recovered from the float-shadow miscount. */
    int16_t object_count = objects_in_sphere(0, object_mask_unit | object_mask_item | object_mask_control, sphere_location, center, 0.1f, object_indices, 16);
    if (object_count <= 0)
        return 0;

    for (int i = 0; i < object_count; i = (int16_t)(i + 1))
    {
        object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                                  object_indices[i])->datum;
        if (object->object.type == object_type_vehicle)          /* object type == vehicle */
            return object != 0;
    }

    return 0;
}
