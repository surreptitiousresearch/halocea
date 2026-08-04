/* create_ghost_effect @0x8375FB28 — spawn the Ghost vehicle's hover-thruster dust/wash effect. For each
 * "hover thrusters" marker on the vehicle a random direction is chosen inside a 15-degree cone about the
 * thruster's forward axis and traced against the world; if it hits a surface, a thrust strength in (0,1] is
 * computed from how far the trace reached (1 - t), how steeply the thruster points down (-forward.z) and the
 * vehicle's current hover intensity (object+824). When positive, an effect is spawned at the impact point with
 * four named markers — incident, normal, reflected and midpoint — describing the wash geometry.
 *
 * DEVIATION 1: seed_random_vector_in_cone3d takes two float cone-angle args that shadow GPR slots
 * (FPR-shadow), so the decompiler lost its result pointer (an uninitialized local); it is the random cone
 * direction, restored from the disassembly.
 * DEVIATION 2: effect_new_unattached_from_markers' two float scale args shadow GPR slots, pushing color,
 * impulse_field and deterministic onto the stack; the decompiler showed them as uninitialized locals. The
 * stores immediately preceding the call (two zeroed words and a byte set to 1) give color = NULL,
 * impulse_field = NULL, deterministic = true. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/object_marker.h"
#include "headers/collision_result.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/collision_test_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern uint32_t *get_global_local_random_seed_address(void);
extern real_vector3d *seed_random_vector_in_cone3d(uint32_t *seed, const real_vector3d *axis, float inner_cone_angle, float outer_cone_angle, real_vector3d *result);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern real_vector3d *reflect_vector3d(const real_vector3d *incident, const real_vector3d *normal, real_vector3d *reflection);
extern int effect_new_unattached_from_markers(int definition_index, int owner_object_index, const real_vector3d *translational_velocity, int16_t marker_count, const char **marker_names, real_point3d *marker_points, real_vector3d *marker_forwards, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field, uint8_t deterministic);

void create_ghost_effect(int vehicle_index)
{
    vehicle_datum *vehicle_object = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, vehicle_object->definition_index);

    int ghost_effect = vehicle_def->vehicle.effect.index;
    if (ghost_effect == -1 || vehicle_object->unit.seat_power[0] <= 0.0f)
        return;

    object_marker markers[16];
    __int16 marker_count = object_get_marker_by_name(vehicle_index, "hover thrusters", markers, 15);
    if (marker_count <= 0)
        return;

    for (int i = 0; i < marker_count; i = (__int16)(i + 1))
    {
        object_marker *marker = &markers[i];

        real_vector3d cone_direction;
        seed_random_vector_in_cone3d(get_global_local_random_seed_address(),
                                     (const real_vector3d *)&marker->matrix.n[0], 0.0f, 15.0f, &cone_direction);

        real_vector3d trace_direction = cone_direction;
        collision_result collision;
        if (!collision_test_vector(_collision_test_environment_flags /* 0x61 */, (const real_point3d *)&marker->matrix.n[3], &trace_direction,
                                   vehicle_index, &collision))
            continue;

        float thrust = (1.0f - collision.t) * (-marker->matrix.n[0][2]) * vehicle_object->unit.seat_power[0];
        if (thrust <= 0.0f)
            continue;
        if (thrust > 1.0f)
            thrust = 1.0f;

        const char *marker_names[4] = { "incident", "normal", "reflected", "midpoint" };

        real_point3d marker_points[4];
        marker_points[0] = collision.point;
        marker_points[1] = collision.point;
        marker_points[2] = collision.point;
        marker_points[3].n[0] = (marker->matrix.n[3][0] + collision.point.n[0]) * 0.5f;
        marker_points[3].n[1] = (marker->matrix.n[3][1] + collision.point.n[1]) * 0.5f;
        marker_points[3].n[2] = (marker->matrix.n[3][2] + collision.point.n[2]) * 0.5f;

        real_vector3d marker_forwards[4];
        marker_forwards[0].n[0] = -cone_direction.n[0]; /* incident (reversed cone direction) */
        marker_forwards[0].n[1] = -cone_direction.n[1];
        marker_forwards[0].n[2] = -cone_direction.n[2];
        marker_forwards[1] = collision.plane.normal;          /* surface normal */
        reflect_vector3d(&cone_direction, &collision.plane.normal, &marker_forwards[2]); /* reflected */
        reflect_vector3d(&cone_direction, &collision.plane.normal, &marker_forwards[3]); /* midpoint */

        effect_new_unattached_from_markers(ghost_effect, -1, nullptr, 4, marker_names, marker_points,
                                           marker_forwards, thrust, thrust, nullptr, nullptr, 1);
    }
}
