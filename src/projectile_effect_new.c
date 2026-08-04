/* projectile_effect_new @0x83759B48 — spawn a projectile impact/detonation effect from the standard 5
 * effect markers: attached to the struck object (with its node) when the collision hit an object
 * (collision type 3), otherwise unattached (deterministic) at the impact point. The hcex_pjl_effect
 * override, when set, replaces the effect definition (hcex bridge debug/feature hook).
 *
 * DEVIATION (bug classes 1+7): the decompiler passed two uninitialized locals (and even `vectors` cast as
 * a color) for the trailing arguments; disasm shows both stacked args are explicit zeros (color = NULL,
 * impulse_field = NULL) plus deterministic = 1 on the unattached path. The DB prototype's `float __noop`
 * 7th parameter is real in the ABI sense: no second float is ever materialized, so the callee's scale_b
 * receives the caller's undefined f2 — kept as a pass-through parameter to reproduce that. */

#include <stdint.h>
#include "headers/collision_result.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/collision_result_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern int effect_new_attached_from_markers(int definition_index, int owner_object_index, int object_index, int16_t node_index, int16_t marker_count, const char **marker_names, const real_point3d *marker_points, const real_vector3d *marker_forwards, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern int effect_new_unattached_from_markers(int definition_index, int owner_object_index, const real_vector3d *translational_velocity, int16_t marker_count, const char **marker_names, real_point3d *marker_points, real_vector3d *marker_forwards, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field, uint8_t deterministic);

void projectile_effect_new(int projectile_index, int definition_index, collision_result *collision,
        real_point3d *points, real_vector3d *vectors, float scale, float __noop)
{
    if ( hcex_pjl_effect != -1 )
        definition_index = hcex_pjl_effect;

    if ( collision->type == collision_result_object )
        effect_new_attached_from_markers(definition_index, projectile_index, collision->object_index,
                                         collision->node_index, 5, effect_marker_names, points, vectors,
                                         scale, __noop, 0, 0);
    else
        effect_new_unattached_from_markers(definition_index, projectile_index, 0, 5, effect_marker_names,
                                           points, vectors, scale, __noop, 0, 0, 1u);
}
