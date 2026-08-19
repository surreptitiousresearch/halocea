/* collision_prism_test_vector @0x83805908 — cast the segment (point → point+vector) against a prism feature by
 * clipping the segment's fraction interval [entry,exit] against the two slab planes (face and face+height) and
 * against each side edge of the convex polygon in the projection plane. On a surviving interval reports the entry
 * fraction with the prism's top-face plane. Returns TRUE on a hit. Faithful transcription. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_plane3d.h"
#include "headers/real_point2d.h"
#include "headers/collision_prism.h"
#include "headers/blam_data_globals.h"


uint8_t collision_prism_test_vector(const collision_prism *prism, const real_point3d *point,
                                    const real_vector3d *vector, float *t, real_plane3d *plane)
{
    float entry = 0.0;
    float exit = 1.0;

    float along = ((prism->plane.n.n[2] * vector->n[2])
                        + ((prism->plane.n.n[0] * vector->n[0])
                                + (prism->plane.n.n[1] * vector->n[1])));
    float above = (((prism->plane.n.n[2] * point->n[2])
                                + ((prism->plane.n.n[0] * point->n[0])
                                        + (prism->plane.n.n[1] * point->n[1])))
                        - prism->plane.d);

    if ( along == 0.0 )
    {
        if ( above < 0.0 || above >= prism->height )
            return 0;
    }
    else
    {
        float inverse = ((float)1.0 / along);
        float t_face = (above * inverse);                       /* crosses the face plane (d=0) */
        float t_top  = -((above - prism->height) * inverse); /* crosses the top plane (d=height) */
        if ( along <= 0.0 )
        {
            if ( t_top > 0.0 )
                entry = t_top;
            if ( -t_face < 1.0 )
                exit = -t_face;
        }
        else
        {
            if ( -t_face > 0.0 )
                entry = -t_face;
            if ( t_top < 1.0 )
                exit = t_top;
        }
        if ( entry > exit )
            return 0;
    }

    int point_count = prism->point_count;
    int16_t ia = global_projection3d_mappings[prism->projection_axis][prism->projection_sign][0];
    int16_t ib = global_projection3d_mappings[prism->projection_axis][prism->projection_sign][1];

    /* projected point (onto the face plane) and projected direction */
    float point_proj[3];
    point_proj[0] = (prism->plane.n.n[0] * -above) + point->n[0];
    point_proj[1] = (prism->plane.n.n[1] * -above) + point->n[1];
    point_proj[2] = (prism->plane.n.n[2] * -above) + point->n[2];
    float dir_proj[3];
    dir_proj[0] = (prism->plane.n.n[0] * -along) + vector->n[0];
    dir_proj[1] = (prism->plane.n.n[1] * -along) + vector->n[1];
    dir_proj[2] = (prism->plane.n.n[2] * -along) + vector->n[2];

    if ( point_count > 0 )
    {
        for ( unsigned int i = 1; (int)(i - 1) < point_count; ++i )
        {
            const real_point2d *current = &prism->points[i - 1];
            const real_point2d *next = &prism->points[(i < (unsigned int)point_count) ? i : 0];

            float denom = (((next->n[1] - current->n[1]) * dir_proj[ia])
                                - (dir_proj[ib] * (next->n[0] - current->n[0])));
            float numerator = (((point_proj[ib] - current->n[1]) * (next->n[0] - current->n[0]))
                                    - ((next->n[1] - current->n[1]) * (point_proj[ia] - current->n[0])));
            if ( denom == 0.0 )
            {
                if ( numerator < 0.0 )
                    return 0;                  /* outside this edge, segment parallel to it */
            }
            else
            {
                float crossing = (numerator / denom);
                if ( denom >= 0.0 )
                {
                    if ( exit > crossing )
                        exit = crossing;
                }
                else if ( entry < crossing )
                {
                    entry = crossing;
                }
                if ( entry > exit )
                    return 0;
            }
        }
    }

    *t = entry;
    plane->n.n[0] = prism->plane.n.n[0];
    plane->n.n[1] = prism->plane.n.n[1];
    plane->n.n[2] = prism->plane.n.n[2];
    plane->d = prism->plane.d + prism->height;
    return 1;
}
