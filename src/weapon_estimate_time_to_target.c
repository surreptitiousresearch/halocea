/* weapon_estimate_time_to_target @0x836D8F10 — estimate the flight time for a weapon's projectile (from
 * the given trigger) to reach a target at target_distance. Returns 0 for an out-of-range trigger.
 *
 *
 * DEVIATION: distance and the return are routed through doubles by the soft-float ABI; they are floats. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/projectile_definition.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/blam_data_globals.h"

extern float projectile_estimate_time_to_target(const projectile_definition *projectile_definition, float target_distance);

float weapon_estimate_time_to_target(int weapon_index, int16_t trigger_index, float target_distance)
{
    weapon_definition *definition = TAG_GET(weapon_definition, DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum->definition_index);
    if ( trigger_index < 0 || trigger_index >= definition->weapon.triggers.count )
        return 0.0f;

    int trigger_projectile_definition_index =
        ((weapon_trigger_definition *)definition->weapon.triggers.address)[trigger_index].projectile.index;
    /* local renamed from projectile_definition to avoid shadowing the type of the same name */
    const projectile_definition *projectile_def =
        TAG_GET(const projectile_definition, trigger_projectile_definition_index);
    return projectile_estimate_time_to_target(projectile_def, target_distance);
}
