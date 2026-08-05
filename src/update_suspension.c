/* update_suspension @0x837600? — update a vehicle's wheel/suspension state each frame and play a tire-roll
 * impulse sound when the suspension compresses sharply. The vehicle's animation graph ('antr' tag,
 * object.animation_graph) supplies suspension animations (stride 20); each one anchored to a valid mass
 * point of the physics tag (object.physics, mass_points stride 128) casts a ray from the mass point's
 * world-space position along its (transformed) up axis to find the ground. The contact fraction
 * gives a 0..1 compression that is smoothed against the previously stored per-wheel compression byte (object
 * +1268+wheel) and re-quantized. The largest jump in compression across all wheels is tracked; if it exceeds
 * 0.3 and the definition has a suspension sound, that sound is started scaled by the excess. Returns 1 if a
 * sound was played, 0 otherwise.
 *
 * DEVIATION: object_impulse_sound_new's float scale shadows a GPR slot (FPR-shadow), so the decompiler left
 * the trailing is_local_player arg uninitialized; the disassembly sets it to 0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/collision_test_flags.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_vehicle_animations.h"
#include "headers/animation_graph_vehicle_suspension_animation.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/collision_result.h"
#include "headers/blam_data_globals.h"


extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern uint8_t quantize_real_to_byte_lower_bound(float min, float max, float value);
extern int object_impulse_sound_new(int object_index, int definition_index, int16_t node_index, const real_point3d *position, const real_vector3d *forward, float scale, uint8_t is_local_player);

uint8_t update_suspension(int vehicle_index)
{
    object_datum *vehicle_object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
    vehicle_datum *vehicle = (vehicle_datum *)vehicle_object;
    vehicle_definition *definition = TAG_GET(vehicle_definition, vehicle_object->definition_index);

    /* DB-corrected: +68 is object.animation_graph.index (the 'antr' tag carries the suspension
     * animations), +140 is object.physics.index (its mass_points are the suspension anchor points).
     * An earlier pass had these two swapped as "physics"/"model". */
    int animation_graph_index = definition->object.animation_graph.index;
    if (animation_graph_index == -1)
        return 0;

    animation_graph *graph = TAG_GET(animation_graph, animation_graph_index);
    if (!graph->vehicle_animations.count)
        return 0;

    animation_graph_vehicle_animations *vehicle_animations =
        (animation_graph_vehicle_animations *)graph->vehicle_animations.address;
    if (!vehicle_animations)
        return 0;

    float max_compression_delta = 0.0f;
    physics_definition *physics = TAG_GET(physics_definition, definition->object.physics.index);

    real_matrix4x3 vehicle_matrix;
    matrix4x3_from_point_and_vectors(&vehicle_matrix, &vehicle_object->object.position,
                                     &vehicle_object->object.forward,
                                     &vehicle_object->object.up);

    if (vehicle_animations->suspension_animations.count > 0)
    {
        for (int wheel = 0; wheel < vehicle_animations->suspension_animations.count; wheel = (int16_t)(wheel + 1))
        {
            animation_graph_vehicle_suspension_animation *suspension_point =
                &((animation_graph_vehicle_suspension_animation *)vehicle_animations->suspension_animations.address)[wheel];
            int node_index = suspension_point->mass_point_index;
            /* sentinel: animation_index is signed __int16 — keep the unsigned compare vs 0xFFFF */
            if (node_index < 0 || node_index >= physics->mass_points.count
                || (uint16_t)suspension_point->animation_index == 0xFFFF)
                continue;

            mass_point_definition *mass_point =
                &((mass_point_definition *)physics->mass_points.address)[node_index];

            /* offset 1268 == vehicle_datum.vehicle.suspension[wheel] (0x4CC + 0x28 + wheel) */
            unsigned char stored = vehicle->vehicle.suspension[wheel];
            float previous_compression = (stored == 255) ? 1.0f : (float)stored * 0.0039215689f;

            real_point3d attach_point;
            real_vector3d suspension_axis;
            matrix4x3_transform_point(&vehicle_matrix, &mass_point->position, &attach_point);
            matrix4x3_transform_normal(&vehicle_matrix, &mass_point->up, &suspension_axis);

            float ray_length = (suspension_point->full_extension_ground_depth
                              - suspension_point->full_compression_ground_depth) * 2.0f;
            float origin_offset = (suspension_point->full_compression_ground_depth - physics->center_of_mass.n[2])
                                - (suspension_point->full_extension_ground_depth
                                 - suspension_point->full_compression_ground_depth);

            real_point3d ray_origin;
            ray_origin.n[0] = suspension_axis.n[0] * origin_offset + attach_point.n[0];
            ray_origin.n[1] = suspension_axis.n[1] * origin_offset + attach_point.n[1];
            ray_origin.n[2] = suspension_axis.n[2] * origin_offset + attach_point.n[2];

            real_vector3d ray_direction;
            ray_direction.n[0] = suspension_axis.n[0] * ray_length;
            ray_direction.n[1] = suspension_axis.n[1] * ray_length;
            ray_direction.n[2] = suspension_axis.n[2] * ray_length;

            collision_result collision;
            collision_test_vector((1u << _collision_test_structure_bit)
                                      | (1u << _collision_test_objects_bit)
                                      | (1u << _collision_test_objects_scenery_bit)
                                      | (1u << _collision_test_objects_machines_bit),
                                  &ray_origin, &ray_direction, vehicle_index, &collision);

            float compression = (1.0f - collision.t) * 2.0f;
            if (compression < 0.0f)
                compression = 0.0f;
            else if (compression > 1.0f)
                compression = 1.0f;

            if (compression - previous_compression > max_compression_delta)
                max_compression_delta = compression - previous_compression;

            vehicle->vehicle.suspension[wheel] =
                quantize_real_to_byte_lower_bound(0.0f, 1.0f, (compression + previous_compression) * 0.5f);
        }
    }

    int suspension_sound = definition->vehicle.suspension_sound.index;
    if (suspension_sound == -1 || max_compression_delta <= 0.30000001f)
        return 0;

    float scale = (max_compression_delta - 0.30000001f) * 1.6666667f;
    if (scale < 0.0f)
        scale = 0.0f;
    else if (scale > 1.0f)
        scale = 1.0f;

    object_impulse_sound_new(vehicle_index, suspension_sound, -1, global_origin3d, global_forward3d, scale, 0);
    return 1;
}
