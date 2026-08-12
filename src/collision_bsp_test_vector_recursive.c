/* collision_bsp_test_vector_recursive @ 0x837E1900 — the recursive worker behind a collision-BSP ray
 * test. Walks the 3D BSP over the parametric interval [t0,t1]: at an interior node it classifies the
 * ray's signed distance to the node plane at both ends and recurses into the near child then, if a
 * closer hit is still possible, the far child (splitting the interval at the plane crossing). At a
 * leaf it detects a contents transition across the surface relative to the previously visited leaf,
 * and if the flags select that transition it tests the leaf's surfaces (collision_leaf_test_vector),
 * recording the nearest hit. Every visited leaf is appended to the result's leaf list. Returns 1 as
 * soon as a surface hit is committed.
 *
 * Leaf contents code (v23): 1 or 2 from the leaf's bit0, 3 = outside the BSP (no leaf).
 * Surface (12 bytes): int plane_designator, byte flags @8, byte breakable_surface_index @9,
 * word material_index @10. */

#include "headers/test_vector_data.h"
#include "headers/collision_surface.h"
#include "headers/collision_bsp.h"
#include "headers/bsp3d_node.h"
#include "headers/collision_bsp_test_vector_result.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_plane3d.h"
#include "headers/collision_bsp_test_flags.h"
#include "headers/collision_surface_flags.h"
#include "headers/collision_leaf.h"
#include "headers/contents.h"
#include <stdint.h>

extern int collision_leaf_test_vector(const collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *point, const real_vector3d *vector, int leaf_index, int plane_index, double t, uint8_t test_surface);

/* Return is 8-bit: the self-recursive callers (0x837E1A40/0x837E1A80/0x837E1AC0) normalize the
 * result with clrlwi rN,r3,24, as does the entry collision_bsp_test_vector. */
uint8_t collision_bsp_test_vector_recursive(test_vector_data *data, int child_index, float t0, float t1)
{
    if ( child_index >= 0 )
    {
        const collision_bsp *bsp = data->bsp;
        const real_point3d *point = data->point;
        const real_vector3d *vector = data->vector;
        int *node = (int *)&((bsp3d_node *)bsp->bsp3d.nodes.address)[child_index];
        float *plane = (float *)&((real_plane3d *)bsp->bsp3d.planes.address)[node[0]];

        float dot_vector = vector->n[0] * plane[0] + (vector->n[1] * plane[1] + vector->n[2] * plane[2]);
        float dist_point = (point->n[0] * plane[0] + (point->n[1] * plane[1] + point->n[2] * plane[2])) - plane[3];
        float d0 = dot_vector * t0 + dist_point;
        float d1 = dot_vector * t1 + dist_point;

        char back_touched = (d0 < 0.0f || d1 < 0.0f) ? 1 : 0;
        unsigned char front_touched = (d0 >= 0.0f || d1 >= 0.0f) ? 1 : 0;

        if ( back_touched && front_touched )
        {
            int front_side = dot_vector > 0.0f;
            double t_mid = -(dist_point / dot_vector);

            /* near child first (the one containing t0) */
            if ( collision_bsp_test_vector_recursive(data, node[(dot_vector <= 0.0f) + 1], t0, t_mid) )
                return 1;
            if ( data->result->t > t_mid )
            {
                data->last_plane_index = node[0];
                if ( collision_bsp_test_vector_recursive(data, node[front_side + 1], t_mid, t1) )
                    return 1;
            }
        }
        else if ( collision_bsp_test_vector_recursive(data, node[front_touched + 1], t0, t1) )
        {
            return 1;
        }
        return 0;
    }

    {
        int leaf_index = -1;
        unsigned char contents = _contents_solid;   /* no leaf: outside the BSP */
        unsigned int flags = data->flags;
        int last_leaf = -1;
        int do_test = 0;

        if ( child_index != -1 )
        {
            leaf_index = child_index & 0x7FFFFFFF;
            contents = (((collision_leaf *)data->bsp->leaves.address)[leaf_index].flags & 1) ? _contents_semi_empty : _contents_empty;
        }

        /* select which contents transition (if any) to resolve a surface for */
        if ( (flags & (1u << _collision_bsp_test_front_facing_surfaces_bit)) != 0 && (data->last_contents == _contents_empty || data->last_contents == _contents_semi_empty) && contents == _contents_solid )
        {
            last_leaf = data->last_leaf_index;
            do_test = 1;
        }
        else if ( (flags & (1u << _collision_bsp_test_back_facing_surfaces_bit)) != 0 && data->last_contents == _contents_solid && (contents == _contents_empty || contents == _contents_semi_empty) )
        {
            last_leaf = leaf_index;
            do_test = 1;
        }
        else if ( (flags & (1u << _collision_bsp_test_ignore_two_sided_surfaces_bit)) == 0 && data->last_contents == _contents_semi_empty && contents == _contents_semi_empty )
        {
            last_leaf = (flags & (1u << _collision_bsp_test_front_facing_surfaces_bit)) ? data->last_leaf_index : leaf_index;
            do_test = 1;
        }

        if ( do_test && last_leaf != -1 )
        {
            /* Binary passes the surface-test gate as the constant 1 (disasm 0x837E1BC4: li r3,1;
             * 0x837E1BD0: stb r3 into the outgoing stack arg), NOT `flags`. The callee reads it with
             * lbz (byte). A prior reconstruction passed `flags` here. */
            int surface_index = collision_leaf_test_vector(data->bsp, data->breakable_surface_count,
                                                           data->breakable_surface_flags, data->point,
                                                           data->vector, last_leaf, data->last_plane_index,
                                                           t0, 1);
            if ( surface_index != -1 )
            {
                collision_surface *surface = &((collision_surface *)data->bsp->surfaces.address)[surface_index];
                if ( ((surface->flags & (1u << _collision_surface_invisible_bit)) == 0 || (data->flags & (1u << _collision_bsp_test_ignore_invisible_surfaces_bit)) == 0)
                  && ((surface->flags & (1u << _collision_surface_breakable_bit)) == 0 || (data->flags & (1u << _collision_bsp_test_ignore_breakable_surfaces_bit)) == 0) )
                {
                    data->result->t = t0;
                    data->result->plane =
                        &((const real_plane3d *)data->bsp->bsp3d.planes.address)[data->last_plane_index];
                    data->result->surface_index = surface_index;
                    data->result->plane_designator = surface->plane_designator;
                    data->result->flags = surface->flags;
                    data->result->breakable_surface_index = surface->breakable_surface_index; /* was raw surface[9] byte offset */
                    data->result->material_index = (surface)->material_index;
                    return 1;
                }
            }
        }

        /* record this leaf in the traversed-leaf list (clamped at 256) */
        if ( leaf_index != -1 )
        {
            collision_bsp_test_vector_result *result = data->result;
            int leaf_count = result->leaf_count;
            if ( leaf_count >= 256 )
                result->leaf_indices[255] = leaf_index;
            else
            {
                result->leaf_indices[leaf_count] = leaf_index;
                ++data->result->leaf_count;
            }
        }

        data->last_leaf_index = leaf_index;
        data->last_contents = contents;
        return 0;
    }
}
