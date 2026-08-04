/* weapon_aim @0x836D8E78 — aim a weapon's trigger at a target. Looks up the weapon's definition, validates
 * the trigger index against the definition's trigger count, fetches that trigger's projectile
 * reference, and runs projectile_aim to
 * compute the firing solution. Returns 1 if a valid trigger was aimed, 0 otherwise.
 * DEVIATION: weapon_aim passes null for projectile_aim's velocity-override / min / forced / result_velocity
 * arguments (verified against the call site). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/projectile_definition.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/blam_data_globals.h"

extern uint8_t projectile_aim(const projectile_definition *projectile_definition, const real_point3d *origin, const real_point3d *target_point, const float *override_velocity_max, float *target_velocity_min, float *target_ballistic_fraction_min, float *forced_velocity, uint8_t lob, real_vector3d *result_aim_vector, float *result_velocity, float *result_ticks, float *result_distance, uint8_t *result_linear);

uint8_t weapon_aim(
    int weapon_index,
    int16_t trigger_index,
    const real_point3d *origin,
    const real_point3d *target_point,
    uint8_t lob,
    real_vector3d *result_aim_vector,
    float *result_ticks,
    float *result_distance,
    uint8_t *result_linear)
{
    object_datum *weapon_object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon_object->definition_index);

    if ( trigger_index >= 0 && trigger_index < definition->weapon.triggers.count )
    {
        int projectile_tag_index = ((weapon_trigger_definition *)definition->weapon.triggers.address)
                                       [trigger_index].projectile.index;
        projectile_definition *projectile = TAG_GET(projectile_definition, projectile_tag_index);
        projectile_aim(projectile, origin, target_point, 0, 0, 0, 0, lob,
            result_aim_vector, 0, result_ticks, result_distance, result_linear);
        return 1;
    }
    return 0;
}
