/* collision_bsp_test_pill_new_recursive @0x837E0CC8 — the "new" swept-pill sweep over a collision BSP: recurse
 * the 3D BSP finding the nearest surface a moving pill (point + vector*t, expanded by radius) crosses along the
 * parametric range [t0, t1]. At an interior node it computes the pill-radius-expanded signed distances of the
 * sweep endpoints to the splitting plane; if the sweep only reaches one side within the radius it descends that
 * child in a loop (no recursion); if it straddles the plane it recurses the near side over the crossing interval
 * first, then, if that near hit is farther than the plane crossing, the far side. On the first empty/solid leaf
 * (child_index == -1) with a recorded crossing plane it writes the hit fraction to *data->t and the (optionally
 * negated) plane normal to *data->normal, returning 1. Returns 1 on any contact.
 *
 * Faithful transcription; dense float span expressions reproduced as emitted (register-pun doubles are single
 * precision). t0/t1 travel in FPRs (the DB `double` is soft-float); the sibling collision_bsp_test_pill_new sets
 * the recursion up. `node[k+1]` selects back_child (k=0) or front_child (k=1). The plane designator's sign bit
 * records whether the sweep faces the plane front (used at the leaf to negate the normal). */

#include <stdint.h>
#include "headers/test_pill_new_data.h"
#include "headers/bsp3d_node.h"
#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

uint8_t collision_bsp_test_pill_new_recursive(test_pill_new_data *data, int child_index, float t0, float t1)
{
    float original_t1 = t1;

    if ( child_index >= 0 )
    {
        const real_point3d  *point  = data->point;
        float                radius = data->radius;
        const real_vector3d *vector = data->vector;
        float                neg_radius = -radius;

        int   *node;
        float  along;         /* rate of change of plane distance along the sweep */
        float  distance_start;
        float  distance_at_t0;
        float  distance_at_t1;
        uint8_t descend_front;

        while ( 1 )
        {
            node = (int *)&((bsp3d_node *)data->bsp->bsp3d.nodes.address)[child_index];
            float *plane = (float *)&((real_plane3d *)data->bsp->bsp3d.planes.address)[*node];

            along = ((vector->n[2] * plane[2])
                    + ((vector->n[0] * plane[0]) + (vector->n[1] * plane[1])));
            distance_start = (((plane[2] * point->n[2])
                            + ((point->n[0] * plane[0]) + (plane[1] * point->n[1])))
                    - plane[3]);
            distance_at_t0 = ((along * t0) + distance_start);
            distance_at_t1 = ((along * t1) + distance_start);

            char reaches_back = (distance_at_t0 < radius || distance_at_t1 < radius);
            descend_front = (distance_at_t0 > neg_radius || distance_at_t1 > neg_radius);
            if ( reaches_back && descend_front )
                break;

            child_index = node[descend_front + 1];
            if ( child_index < 0 )
                goto leaf;
        }

        float crossing_far;   /* v33 — the far crossing fraction, clamped to [t0, original_t1] */
        if ( along == 0.0f )
        {
            crossing_far = t0;
        }
        else
        {
            float inverse = 1.0f / along;
            float back_cross = ((data->radius + distance_start) * inverse);
            float front_cross = ((distance_start - data->radius) * inverse);
            /* the near interval [-max, ...] used to bound the near recursion; far crossing bounds the far side */
            t1 = -back_cross <= -front_cross ? -front_cross : -back_cross;
            crossing_far = -back_cross <= -front_cross ? -back_cross : -front_cross;

            if ( t1 >= t0 )
            {
                if ( t1 > original_t1 )
                    t1 = original_t1;
            }
            else
            {
                t1 = t0;
            }

            if ( crossing_far < t0 )
                crossing_far = t0;
            else if ( crossing_far > original_t1 )
                crossing_far = original_t1;
        }

        BOOL facing_front = along > 0.0f;
        int near_hit = collision_bsp_test_pill_new_recursive(data, node[(along <= 0.0f) + 1], t0, t1);
        if ( near_hit )
        {
            if ( crossing_far >= *data->t )
                return 1;
            original_t1 = *data->t;
        }

        data->last_plane_designator = facing_front ? (*node | 0x80000000) : (*node & 0x7FFFFFFF);
        return collision_bsp_test_pill_new_recursive(data, node[facing_front + 1], crossing_far, original_t1)
                | (uint8_t)near_hit;
    }

leaf:
    if ( child_index != -1 )
        return 0;
    if ( data->last_plane_designator == -1 )
        return 0;

    /* DEVIATION: designator bit 31 is the facing flag; the binary's 16-byte stride shift discards it
     * (slwi r10,r11,4 @0x837E0DF8) and tests it separately (clrrwi r6,r7,31 @0x837E0E0C) — x64 no-op */
    float *plane = (float *)&((real_plane3d *)data->bsp->bsp3d.planes.address)[data->last_plane_designator & 0x7FFFFFFF];
    *data->t = t0;
    real_vector3d *normal = data->normal;
    if ( data->last_plane_designator < 0 )
    {
        normal->n[0] = -plane[0];
        normal->n[1] = -plane[1];
        normal->n[2] = -plane[2];
    }
    else
    {
        normal->n[0] = plane[0];
        normal->n[1] = plane[1];
        normal->n[2] = plane[2];
    }
    return 1;
}
