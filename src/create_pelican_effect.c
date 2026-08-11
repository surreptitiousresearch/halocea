/* create_pelican_effect @0x8375F860 — spawn the Pelican dropship's thruster wash effects. Collects both the
 * "hover thrusters" and "jet thrusters" markers on the vehicle into one array, and for each one chooses a
 * random direction inside a ~15-degree cone about the thruster's forward axis and traces it against the world
 * along a distance scaled by that thruster's current intensity (object+1260 hover / object+1264 jet, x6 + 2).
 * On a hit it spawns an effect at the impact point with three named markers — incident, normal, reflected —
 * scaled by how far the trace reached (1 - t), and notifies halo_update_pelican_effects of the per-thruster
 * impact (named "<type>_<index>"); on a miss it still notifies with a null point/normal.
 *
 * DEVIATION 1: seed_random_vector_in_cone3d's two float cone-angle args shadow GPR slots (FPR-shadow), so the
 * decompiler lost its result pointer (an uninitialized local); it is the random cone direction, restored from
 * the disassembly.
 * DEVIATION 2: effect_new_unattached_from_markers' two float scale args shadow GPR slots, pushing color,
 * impulse_field and deterministic onto the stack; the decompiler showed them as uninitialized locals. The
 * stores preceding the call (two zeroed words and a byte set to 1) give color = NULL, impulse_field = NULL,
 * deterministic = true. The three marker_forwards the decompiler split across v25/v26+v27/v28[0] are one
 * contiguous 3-element array. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/collision_test_flags.h"
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
#include "headers/blam_data_globals.h"


#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern int sprintf_0(char *string, const char *format, ...);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern uint32_t *get_global_local_random_seed_address(void);
extern real_vector3d *seed_random_vector_in_cone3d(uint32_t *seed, const real_vector3d *axis, float inner_cone_angle, float outer_cone_angle, real_vector3d *result);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern real_vector3d *reflect_vector3d(const real_vector3d *incident, const real_vector3d *normal, real_vector3d *reflection);
extern int effect_new_unattached_from_markers(int definition_index, int owner_object_index, const real_vector3d *translational_velocity, int16_t marker_count, const char **marker_names, real_point3d *marker_points, real_vector3d *marker_forwards, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field, uint8_t deterministic);
extern void halo_update_pelican_effects(int vehicle_index, const char *src_obj, real_point3d *point, real_vector3d *vector, int start);

void create_pelican_effect(int vehicle_index)
{
    vehicle_datum *vehicle_object = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, vehicle_object->definition_index);

    int pelican_effect = vehicle_def->vehicle.effect.index;
    if (pelican_effect == -1)
        return;

    object_marker markers[16];
    int16_t hover_count = object_get_marker_by_name(vehicle_index, "hover thrusters", markers, 15);
    int16_t total_count = object_get_marker_by_name(vehicle_index, "jet thrusters", &markers[hover_count],
                                                    16 - hover_count) + hover_count;
    if (total_count <= 0)
        return;

    for (int i = 0; i < total_count; i = (int16_t)(i + 1))
    {
        object_marker *marker = &markers[i];

        const char *thruster_type;
        int thruster_local_index;
        if (i < hover_count)
        {
            thruster_type = "hover thrusters";
            thruster_local_index = i;
        }
        else
        {
            thruster_type = "jet thrusters";
            thruster_local_index = i - hover_count;
        }
        char thruster_name[32];
        sprintf_0(thruster_name, "%s_%d", thruster_type, thruster_local_index);

        real_vector3d cone_direction;
        seed_random_vector_in_cone3d(get_global_local_random_seed_address(),
                                     (const real_vector3d *)&marker->matrix.n[0], 0.0f, 0.2617994f, /* ~15 deg */
                                     &cone_direction);

        /* Trace distance scales with the thruster's current intensity. */
        float thruster_intensity = (i >= hover_count) ? vehicle_object->vehicle.thrust
                                                      : vehicle_object->vehicle.hover;
        float trace_length = thruster_intensity * 6.0f + 2.0f;
        real_vector3d trace_vector;
        trace_vector.n[0] = cone_direction.n[0] * trace_length;
        trace_vector.n[1] = trace_length * cone_direction.n[1];
        trace_vector.n[2] = cone_direction.n[2] * trace_length;

        collision_result collision;
        real_point3d *impact_point;
        real_vector3d *impact_normal;
        int hit;
        if (collision_test_vector(_collision_test_environment_flags /* 0x61 */, (const real_point3d *)&marker->matrix.n[3], &trace_vector,
                                  vehicle_index, &collision))
        {
            real_vector3d marker_forwards[3];
            marker_forwards[0].n[0] = -cone_direction.n[0]; /* incident (reversed cone direction) */
            marker_forwards[0].n[1] = -cone_direction.n[1];
            marker_forwards[0].n[2] = -cone_direction.n[2];
            marker_forwards[1] = collision.plane.normal;    /* surface normal */
            reflect_vector3d(&cone_direction, &collision.plane.normal, &marker_forwards[2]); /* reflected */

            const char *marker_names[3] = { "incident", "normal", "reflected" };
            real_point3d marker_points[3];
            marker_points[0] = collision.point;
            marker_points[1] = collision.point;
            marker_points[2] = collision.point;

            float effect_scale = 1.0f - collision.t;
            effect_new_unattached_from_markers(pelican_effect, -1, nullptr, 3, marker_names, marker_points,
                                               marker_forwards, effect_scale, effect_scale, nullptr, nullptr, 1);

            hit = 1;
            impact_normal = &collision.plane.normal;
            impact_point = &collision.point;
        }
        else
        {
            hit = 0;
            impact_normal = nullptr;
            impact_point = nullptr;
        }

        halo_update_pelican_effects(vehicle_index, thruster_name, impact_point, impact_normal, hit);
    }
}
