/* collision_features_from_surface @0x83804C10 — turn one collision-BSP surface into a polygon collision feature.
 * Gathers the surface polygon vertices and its (orientation-corrected) plane, transforms both by `matrix` if
 * given, and forwards to collision_features_from_polygon.
 *
 * Deviation: float-GPR-skip scrambles the Hex-Rays names (and invents `a8` == object_index). True register map
 * (from the prologue): r3=bsp, r4=surface_index, r5=matrix, f1=height, f2=width, r8=object_index, r9=features.
 * The forwarded call maps to from_polygon(point_count, points, plane, height, width, object_index, surface_index,
 * surface.flags, surface.breakable_surface_index, surface.material_index, features) where the reported
 * surface_index is (object_index==-1) ? surface_index : -1. The decompiler's `SLODWORD(v15)` object_index
 * argument is the plane distance float misread and is dropped. */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/collision_surface.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/collision_feature_list.h"

extern int16_t collision_surface_polygon(const collision_bsp *bsp, int surface_index, real_point3d *points);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_plane3d *matrix4x3_transform_plane(const real_matrix4x3 *matrix, const real_plane3d *plane, real_plane3d *result);
extern void collision_features_from_polygon(int16_t point_count, const real_point3d *points, const real_plane3d *plane, float height, float width, int object_index, int surface_index, uint8_t flags, uint8_t breakable_surface_index, int16_t material_index, collision_feature_list *features);

void collision_features_from_surface(const collision_bsp *bsp, int surface_index, const real_matrix4x3 *matrix,
                                     float height, float width, int object_index, collision_feature_list *features)
{
    const collision_surface *surface = &((const collision_surface *)bsp->surfaces.address)[surface_index];

    real_point3d points[8];
    int16_t point_count = collision_surface_polygon(bsp, surface_index, points);

    const float *plane_data = (const float *)&((const real_plane3d *)bsp->bsp3d.planes.address)[surface->plane_designator];
    real_plane3d plane;
    if ( surface->plane_designator >= 0 )
    {
        plane.n = *(const real_vector3d *)plane_data;
        plane.d = plane_data[3];
    }
    else
    {
        plane.n.n[0] = -plane_data[0];
        plane.n.n[1] = -plane_data[1];
        plane.n.n[2] = -plane_data[2];
        plane.d = -plane_data[3];
    }

    if ( matrix )
    {
        for ( int i = 0; i < point_count; i = (int16_t)(i + 1) )
            matrix4x3_transform_point(matrix, &points[i], &points[i]);
        matrix4x3_transform_plane(matrix, &plane, &plane);
    }

    int reported_surface_index = (object_index == -1) ? surface_index : -1;

    collision_features_from_polygon(point_count, points, &plane, height, width, object_index,
                                    reported_surface_index, surface->flags,
                                    surface->breakable_surface_index, surface->material_index, features);
}
