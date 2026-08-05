/* actor_move_avoidance_setup @0x837C7CF8 — collect the obstacle cylinders an AI mover must steer around. Runs an
 * objects_in_sphere query around the mover's origin (out to avoid_distance), then for every found object other
 * than the mover itself, resolves the object's model definition and, if it carries pathfinding spheres, computes
 * the object's footprint radius: the max over its pathfinding spheres of (2D distance from the object origin to
 * the world/node-transformed sphere center + the sphere's scaled radius). Each qualifying object becomes a
 * vehicle_avoidance_cylinder (base xy from the object origin, base z / height derived from the object's vertical
 * extent minus the footprint radius, width = footprint radius), appended to avoidance_data->avoidance_objects up
 * to the 1024-entry cap.
 *
 * DEVIATION: the objects_in_sphere call is an FPR-shadow ABI trap — the float `radius` (avoid_distance, in f1)
 * burns the r7 GPR shadow slot, so Hex-Rays mis-read the trailing integer args. Disasm (0x837C7D34-0x837C7D5C:
 * r8=&buffer, r9=0x800) confirms the DB's 7-arg prototype: object_indices is the local buffer and maximum_count
 * is 2048 (Hex-Rays instead showed the leftover 3*object_index index computation as object_indices). The buffer
 * is int[2048], not the 160-byte array Hex-Rays inferred. v8/max_radius seeds from f27 (0.0, via `fmr f31,f27`),
 * which is also the `fsel` height floor. The v13 int-store/float-load pun is just *(float*)(object+168). */

#include <stdint.h>
#include "headers/vector_avoidance_data.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/_object_definition.h"
#include "headers/collision_model.h"
#include "headers/pathfinding_sphere.h"
#include "headers/global_tag_instances.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/location.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);


extern int16_t objects_in_sphere(unsigned int class_flags, unsigned int type_flags, const location *location, const real_point3d *center, float radius, int *object_indices, int16_t maximum_count);
extern real_matrix4x3 *object_get_world_matrix(int object_index, real_matrix4x3 *matrix);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);


void actor_move_avoidance_setup(vector_avoidance_data *avoidance_data)
{
    int found_objects[2048];
    object_datum *mover = (object_datum *)((char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, avoidance_data->object_index)->datum);

    int found_count = objects_in_sphere(1, object_mask_sightblocking, &mover->object.location,
            &avoidance_data->origin, avoidance_data->avoid_distance, found_objects, 2048);

    avoidance_data->avoidance_object_count = 0;
    if ( found_count <= 0 )
        return;

    for ( int i = 0; i < found_count; ++i )
    {
        int object_index = found_objects[i];
        if ( object_index == -1 || object_index == avoidance_data->object_index )
            continue;

        object_datum *object = (object_datum *)((char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
        const _object_definition *object_definition =
            TAG_GET(const _object_definition, object->definition_index);
        int model_index = object_definition->collision_model.index;
        const collision_model *model_definition = TAG_GET(const collision_model, model_index);

        int sphere_count = model_definition->pathfinding_spheres.count;
        if ( sphere_count <= 0 )
            continue;

        float object_x = object->object.bounding_sphere_center.n[0];
        float object_y = object->object.bounding_sphere_center.n[1];
        float object_z = object->object.bounding_sphere_center.n[2];
        float object_top = object->object.bounding_sphere_radius;

        real_matrix4x3 world_matrix;
        object_get_world_matrix(object_index, &world_matrix);

        float max_radius = 0.0f;
        const pathfinding_sphere *spheres = (const pathfinding_sphere *)model_definition->pathfinding_spheres.address;
        for ( int s = 0; s < sphere_count; ++s )
        {
            const pathfinding_sphere *sphere = &spheres[s];
            uint16_t node_index = (uint16_t)sphere->node_index;

            real_point3d center;
            float scale;
            if ( node_index == 0xFFFF )
            {
                matrix4x3_transform_point(&world_matrix, &sphere->center, &center);
                scale = world_matrix.scale;
            }
            else
            {
                const real_matrix4x3 *node_matrix = object_get_node_matrix(object_index, node_index);
                matrix4x3_transform_point(node_matrix, &sphere->center, &center);
                scale = node_matrix->scale;
            }

            float radius = __fsqrts((center.n[0] - object_x) * (center.n[0] - object_x)
                            + (center.n[1] - object_y) * (center.n[1] - object_y))
                    + sphere->radius * scale;
            if ( max_radius <= radius )
                max_radius = radius;
        }

        int16_t count = avoidance_data->avoidance_object_count;
        if ( count < 1024 )
        {
            avoidance_data->avoidance_object_count = count + 1;
            vehicle_avoidance_cylinder *cylinder = &avoidance_data->avoidance_objects[count];
            float double_height = 2.0f * object_top - 2.0f * max_radius;
            cylinder->object_index = object_index;
            cylinder->base.n[0] = object_x;
            cylinder->base.n[1] = object_y;
            cylinder->base.n[2] = object_z - (object_top - max_radius);
            cylinder->height = double_height >= 0.0f ? double_height : 0.0f;
            cylinder->width = max_radius;
        }
    }
}
