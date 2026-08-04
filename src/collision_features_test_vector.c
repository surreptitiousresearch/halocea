/* collision_features_test_vector @0x83805BC0 — cast the segment (point → point+vector) against every gathered
 * collision feature, keeping the nearest forward-facing contact (smallest fraction whose plane opposes the
 * vector). The winning feature's plane, contact point and metadata are written into `collision`. If nothing is
 * hit, reports t=1.0 with the segment endpoint. Returns TRUE on a hit. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_plane3d.h"
#include "headers/collision_feature_list.h"
#include "headers/collision_plane.h"

extern uint8_t collision_sphere_test_vector(const collision_sphere *sphere, const real_point3d *point, const real_vector3d *vector, float *t, real_plane3d *plane);
extern uint8_t collision_cylinder_test_vector(const collision_cylinder *cylinder, const real_point3d *point, const real_vector3d *vector, float *t, real_plane3d *plane);
extern uint8_t collision_prism_test_vector(const collision_prism *prism, const real_point3d *point, const real_vector3d *vector, float *t, real_plane3d *plane);

uint8_t collision_features_test_vector(const collision_feature_list *features, const real_point3d *point,
                                       const real_vector3d *vector, collision_plane *collision)
{
    float best_t = 3.4028235e38;
    __int16 best_kind = -1;
    __int16 best_index = -1;
    real_plane3d best_plane;

    for ( int kind = 0; kind < 3; kind = (__int16)(kind + 1) )
    {
        for ( __int16 index = 0; index < features->count[kind]; index = (__int16)(index + 1) )
        {
            float fraction[4];
            real_plane3d plane;
            unsigned __int8 hit;
            if ( kind == 0 )
                hit = collision_sphere_test_vector(&features->spheres[index], point, vector, fraction, &plane);
            else if ( kind == 1 )
                hit = collision_cylinder_test_vector(&features->cylinders[index], point, vector, fraction, &plane);
            else
                hit = collision_prism_test_vector(&features->prisms[index], point, vector, fraction, &plane);

            if ( hit && best_t > fraction[0]
              && ((vector->n[0] * plane.n.n[0])
                       + ((vector->n[1] * plane.n.n[1])
                               + (vector->n[2] * plane.n.n[2]))) < -0.000099999997 )
            {
                best_t = fraction[0];
                best_kind = kind;
                best_index = index;
                best_plane = plane;
            }
        }
    }

    if ( best_kind == -1 )
    {
        collision->t = 1.0;
        collision->point.n[0] = point->n[0] + vector->n[0];
        collision->point.n[1] = point->n[1] + vector->n[1];
        collision->point.n[2] = point->n[2] + vector->n[2];
        return 0;
    }

    collision->t = best_t;
    collision->point.n[0] = (vector->n[0] * best_t) + point->n[0];
    collision->point.n[1] = (vector->n[1] * best_t) + point->n[1];
    collision->point.n[2] = (vector->n[2] * best_t) + point->n[2];
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
