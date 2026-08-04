/* physics_get_features_in_sphere @0x837BDFA8 — emit a point collision feature for each of an object's physics
 * mass points (transformed into world space), growing the feature radius by the mass point's own radius. Returns
 * whether any feature (sphere/cylinder/prism) was produced. The `center`/`radius` query parameters are unused
 * here — every mass point is always added.
 *
 * Deviation: float-GPR-skip scrambles the from_point argument names; true mapping (from disasm) is
 * from_point(point, height, width, object_index=instance->object_index, surface_index=-1, flags=0,
 * breakable_surface_index=0xFF, material_index=-1, features). The decompiler split `features` into a phantom
 * `a9` for the final count test — it is features->count[]. */

#include <stdint.h>
#include "headers/physics_instance.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/collision_feature_list.h"

extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern void collision_features_from_point(const real_point3d *point, float height, float width, int object_index, int surface_index, uint8_t flags, uint8_t breakable_surface_index, int16_t material_index, collision_feature_list *features);

uint8_t physics_get_features_in_sphere(const physics_instance *instance, const real_point3d *center, float radius,
                                       float height, float width, collision_feature_list *features)
{
    const physics_definition *physics = instance->physics;
    if ( physics->mass_points.count > 0 )
    {
        const real_matrix4x3 *world_matrix = &instance->world_matrix;
        for ( int i = 0; i < physics->mass_points.count; i = (__int16)(i + 1) )
        {
            const mass_point_definition *mass_point =
                &((const mass_point_definition *)physics->mass_points.address)[i];
            real_point3d world_position;
            matrix4x3_transform_point(world_matrix, &mass_point->position, &world_position);
            collision_features_from_point(&world_position, height,
                                          ((mass_point->radius * world_matrix->scale) + width),
                                          instance->object_index, -1, 0, 0xFF, -1, features);
            physics = instance->physics;
        }
    }

    return features->count[0] || features->count[1] || features->count[2];
}
