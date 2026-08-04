/* find_aim_assist_targets_recursive @0x836C6000 — walk an object and its child/sibling chain collecting
 * aim-assist targets that fall inside the aiming cone. For each object whose bounding sphere
 * (center +160, radius +172) intersects the cone (apex = position, axis = direction), a living enemy
 * unit (type bit 0 at +180, not dead per flag 4 at +262, not the ignore object, on an enemy team, and
 * whose definition does not forbid aim assist via flag 0x200000) is scored by aim_assist_compute_target
 * and appended to targets. Each qualifying object's first child (+280) is recursed into, and the
 * iteration advances along the sibling link (+276). Returns the number of targets written (bounded by
 * maximum_target_count).
 *
 * DEVIATION: the DB prototype (11 args) is ground truth; the decompiler invented ~24 phantom trailing
 * args from the FPR-shadow of the three float args (distance/sine/cosine). Both scrambled calls were
 * recovered from disasm:
 *   sphere_intersects_cone3d(center=+160, radius=+172, base=position, direction, distance, sine, cosine)
 *   the recursive call forwards (parameters, child, position, direction, distance, sine, cosine,
 *     ignore_object_index, ignore_team_index, maximum_target_count - found, &targets[found]). */

#include <stdint.h>
#include "headers/aim_assist_parameters.h"
#include "headers/aim_assist_target.h"
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/object_type.h"
#include "headers/unit_definition_flags.h"
#include "headers/blam_data_globals.h"


extern uint8_t sphere_intersects_cone3d(const real_point3d *center, float radius, const real_point3d *base, const real_vector3d *direction, float distance, float sine, float cosine);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern uint8_t aim_assist_compute_target(const aim_assist_parameters *parameters, int object_index, const real_point3d *position, const real_vector3d *direction, aim_assist_target *target);
extern void *memcpy(void *destination, const void *source, unsigned int count);

__int16 find_aim_assist_targets_recursive(const aim_assist_parameters *parameters, int object_index,
                                          const real_point3d *position, const real_vector3d *direction,
                                          float distance, float angle_sine, float angle_cosine,
                                          int ignore_object_index, __int16 ignore_team_index,
                                          __int16 maximum_target_count, aim_assist_target *targets)
{
    int found = 0;
    do
    {
        unit_datum *object =
            (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        int object_type_mask = 1 << object->object.type;

        if ((object_type_mask & object_mask_unit) != 0
            && (object->object.flags & (1u << _object_invisible_bit)) == 0
            && object->unit.active_camouflage < 1.0f
            && sphere_intersects_cone3d(&object->object.bounding_sphere_center, object->object.bounding_sphere_radius,
                                        position, direction, distance, angle_sine, angle_cosine))
        {
            if (((1 << object->object.type) & object_mask_biped) != 0
                && (object->object.damage_flags & (1u << _object_dead_bit)) == 0
                && object_index != ignore_object_index)
            {
                if (game_team_is_enemy(ignore_team_index, object->object.owner_team_index))
                {
                    unit_definition *object_definition = TAG_GET(unit_definition, object->definition_index);
                    int object_definition_flags = object_definition->unit.flags;  /* +380 */
                    aim_assist_target candidate;
                    if ((object_definition_flags & (1u << _unit_ignored_by_autoaiming)) == 0
                        && aim_assist_compute_target(parameters, object_index, position, direction, &candidate)
                        && (__int16)found < maximum_target_count)
                    {
                        memcpy(&targets[(__int16)found], &candidate, sizeof(aim_assist_target));
                        found = (__int16)(found + 1);
                    }
                }
            }

            int child_object = object->object.first_child_object_index;
            if (child_object != -1 && (__int16)found < maximum_target_count)
                found = (__int16)(find_aim_assist_targets_recursive(
                                      parameters, child_object, position, direction,
                                      distance, angle_sine, angle_cosine,
                                      ignore_object_index, ignore_team_index,
                                      maximum_target_count - found, &targets[found])
                                  + found);
        }
        object_index = object->object.next_object_index;
    } while (object_index != -1 && (__int16)found < maximum_target_count);

    return found;
}
