/* collision_features_test_point @0x83805278 — test a point against every gathered collision feature, keeping the
 * deepest penetration (largest reported depth). Spheres, then cylinders, then prisms are tested via their
 * respective primitive point tests; the winning feature's plane and metadata are copied into `collision`.
 * Returns TRUE if any feature contained the point. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/collision_feature_list.h"
#include "headers/collision_plane.h"

extern uint8_t collision_sphere_test_point(const collision_sphere *sphere, const real_point3d *point, float *depth, real_plane3d *plane);
extern uint8_t collision_cylinder_test_point(const collision_cylinder *cylinder, const real_point3d *point, float *depth, real_plane3d *plane);
extern uint8_t collision_prism_test_point(const collision_prism *prism, const real_point3d *point, float *depth, real_plane3d *plane);

uint8_t collision_features_test_point(const collision_feature_list *features, const real_point3d *point,
                                      collision_plane *collision)
{
    int16_t best_kind = -1;
    float best_depth = -3.4028235e38;
    int16_t best_index = -1;
    real_plane3d best_plane;

    for ( int kind = 0; kind < 3; kind = (int16_t)(kind + 1) )
    {
        for ( int16_t index = 0; index < features->count[kind]; index = (int16_t)(index + 1) )
        {
            float depth[4];
            real_plane3d plane;
            uint8_t hit;
            if ( kind == 0 )
                hit = collision_sphere_test_point(&features->spheres[index], point, depth, &plane);
            else if ( kind == 1 )
                hit = collision_cylinder_test_point(&features->cylinders[index], point, depth, &plane);
            else
                hit = collision_prism_test_point(&features->prisms[index], point, depth, &plane);

            if ( hit && best_depth < depth[0] )
            {
                best_depth = depth[0];
                best_kind = kind;
                best_index = index;
                best_plane = plane;
            }
        }
    }

    if ( best_kind == -1 )
        return 0;

    collision->t = best_depth;
    collision->plane = best_plane;

    if ( best_kind == 0 )
    {
        const collision_sphere *feature = &features->spheres[best_index];
        collision->object_index = feature->object_index;
        collision->surface_index = feature->surface_index;
        collision->flags = feature->flags;
        collision->breakable_surface_index = feature->breakable_surface_index;
        collision->material_index = feature->material_index;
    }
    else if ( best_kind == 1 )
    {
        const collision_cylinder *feature = &features->cylinders[best_index];
        collision->object_index = feature->object_index;
        collision->surface_index = feature->surface_index;
        collision->flags = feature->flags;
        collision->breakable_surface_index = feature->breakable_surface_index;
        collision->material_index = feature->material_index;
    }
    else
    {
        const collision_prism *feature = &features->prisms[best_index];
        collision->object_index = feature->object_index;
        collision->surface_index = feature->surface_index;
        collision->flags = feature->flags;
        collision->breakable_surface_index = feature->breakable_surface_index;
        collision->material_index = feature->material_index;
    }
    return 1;
}
