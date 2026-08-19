/* hs_damage_object @0x837F8708 — apply a damage-effect definition directly to a single object, centred on the
 * object's own origin. */

#include <stdint.h>
#include "headers/damage_data.h"
#include "headers/real_point3d.h"

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern void damage_data_new(damage_data *damage_data, int definition_index);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);

void hs_damage_object(int definition_index, int object_index)
{
    if ( object_index == -1 )
        return;

    damage_data damage;
    damage_data_new(&damage, definition_index);
    object_get_origin(object_index, &damage.origin);
    damage.epicenter = damage.origin;
    scenario_location_from_point(&damage.location, &damage.origin);
    object_cause_damage(&damage, object_index, -1, -1, -1, nullptr);
}
