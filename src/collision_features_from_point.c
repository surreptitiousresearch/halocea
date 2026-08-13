/* collision_features_from_point @0x838040F0 — convert a single contact point (with optional vertical extent
 * `height` and radius `width`) into collision features appended to `features`: always a sphere at `point`, and
 * when height>0 additionally a lower sphere at point.z-height plus a cylinder spanning the two.
 *
 * Deviation: the Hex-Rays output is scrambled by the float-GPR-skip ABI (height/width occupy FPRs), which shifts
 * every scalar parameter name by one and invents a phantom `a30` that is the real `features` pointer. The DB
 * 9-arg prototype is authoritative; the register->param mapping below is reconstructed from the store offsets
 * (e.g. decompiler "flags" writes sphere.object_index, so it is really object_index, etc.). */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/collision_feature_list.h"
#include "headers/collision_feature.h"

void collision_features_from_point(const real_point3d *point, float height, float width,
                                   int object_index, int surface_index, uint8_t flags,
                                   uint8_t breakable_surface_index, int16_t material_index,
                                   collision_feature_list *features)
{
    if ( features->count[_collision_feature_sphere] < 256 )
    {
        collision_sphere *sphere = &features->spheres[features->count[_collision_feature_sphere]++];
        sphere->object_index = object_index;
        sphere->surface_index = surface_index;
        sphere->flags = flags;
        sphere->breakable_surface_index = breakable_surface_index;
        sphere->material_index = material_index;
        sphere->center.n[0] = point->n[0];
        sphere->center.n[1] = point->n[1];
        sphere->center.n[2] = point->n[2];
        sphere->radius = width;
    }

    if ( height > 0.0 )
    {
        float lowered_z = (point->n[2] - height);

        if ( features->count[_collision_feature_sphere] < 256 )
        {
            collision_sphere *sphere = &features->spheres[features->count[_collision_feature_sphere]++];
            sphere->object_index = object_index;
            sphere->surface_index = surface_index;
            sphere->flags = flags;
            sphere->breakable_surface_index = breakable_surface_index;
            sphere->material_index = material_index;
            sphere->center.n[0] = point->n[0];
            sphere->center.n[1] = point->n[1];
            sphere->center.n[2] = lowered_z;
            sphere->radius = width;
        }

        if ( features->count[_collision_feature_cylinder] < 256 )
        {
            collision_cylinder *cylinder = &features->cylinders[features->count[_collision_feature_cylinder]++];
            cylinder->object_index = object_index;
            cylinder->surface_index = surface_index;
            cylinder->flags = flags;
            cylinder->breakable_surface_index = breakable_surface_index;
            cylinder->material_index = material_index;
            cylinder->base.n[0] = point->n[0];
            cylinder->base.n[1] = point->n[1];
            cylinder->base.n[2] = lowered_z;
            cylinder->height.n[0] = 0.0;
            cylinder->height.n[1] = 0.0;
            cylinder->height.n[2] = height;
            cylinder->width = width;
        }
    }
}
