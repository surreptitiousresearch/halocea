/* collision_features_from_polygon @0x838046E8 — build a single prism collision feature from a polygon (its
 * vertices, supporting plane, extrusion `height` and thickness `width`). The polygon points are projected to the
 * prism's dominant-axis 2D plane; when height>0 and the plane faces downward the prism is extruded down by
 * `height` (plane distance shifted and the world-z 2D component of every point lowered).
 *
 * Deviation: identical float-GPR-skip scramble to collision_features_from_point — the parameter names are
 * shifted by one and `features` arrives on the stack (decompiler `a34`). True mapping: object_index=decompiler
 * flags, surface_index=decompiler breakable_surface_index, flags=decompiler material_index,
 * breakable_surface_index=decompiler a30, material_index=decompiler a32, features=a34.
 * DEVIATION: the decompiler flattened global_projection3d_mappings to [0][2*axis+sign][c]; the DB
 * applied type @0x821145B4 is const __int16[3][2][2], so it is indexed [axis][sign][c] here. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/collision_feature_list.h"
#include "headers/blam_data_globals.h"

extern double __fabs(double x);

void collision_features_from_polygon(int16_t point_count, const real_point3d *points, const real_plane3d *plane,
                                     float height, float width, int object_index, int surface_index,
                                     uint8_t flags, uint8_t breakable_surface_index,
                                     int16_t material_index, collision_feature_list *features)
{
    if ( features->count[2] >= 256 )
        return;

    collision_prism *prism = &features->prisms[features->count[2]++];
    prism->object_index = object_index;
    prism->surface_index = surface_index;
    prism->flags = flags;
    prism->breakable_surface_index = breakable_surface_index;
    prism->material_index = material_index;
    prism->plane.n = plane->n;
    prism->plane.d = plane->d;
    prism->height = width;

    float abs_y = __fabs(prism->plane.n.n[1]);
    float abs_z = __fabs(prism->plane.n.n[2]);
    float abs_x = __fabs(prism->plane.n.n[0]);
    prism->projection_axis = (abs_z < abs_y || abs_z < abs_x) ? (abs_y >= abs_x) : 2;
    prism->projection_sign = (prism->plane.n.n[prism->projection_axis] > 0.0);
    prism->point_count = point_count;

    for ( int i = 0; i < prism->point_count; i = (int16_t)(i + 1) )
    {
        prism->points[i].n[0] = points[i].n[global_projection3d_mappings[prism->projection_axis][prism->projection_sign][0]];
        prism->points[i].n[1] = points[i].n[global_projection3d_mappings[prism->projection_axis][prism->projection_sign][1]];
    }

    if ( height > 0.0 && plane->n.n[2] < 0.0 )
    {
        prism->plane.d = -((prism->plane.n.n[2] * height) - prism->plane.d);
        if ( prism->projection_axis != 2 )
        {
            int lowered_component =
                (global_projection3d_mappings[prism->projection_axis][prism->projection_sign][1] == 2);
            for ( int i = 0; i < prism->point_count; i = (int16_t)(i + 1) )
                prism->points[i].n[lowered_component] = (prism->points[i].n[lowered_component] - height);
        }
    }
}
