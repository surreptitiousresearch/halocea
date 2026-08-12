/* collision_bsp_test_vector @ 0x837E22D0 — entry point for a ray test against a collision BSP. Packs
 * the ray, filter flags and breakable-surface state into a test_vector_data, clears the result,
 * clamps the search interval to [0,1] of the (already non-negative) maximum t, and kicks off the
 * recursive node walk that fills in the nearest hit and the list of leaves crossed.
 *
 * Deviation: the decompiler showed a spurious extra trailing result pointer (PPC register/ABI
 * artifact); there is a single result block. The fsel that seeds result->t is the maximum t. */

#include <stdint.h>
#include "headers/test_vector_data.h"
#include "headers/collision_bsp_test_vector_result.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/contents.h"

struct collision_bsp;

extern uint8_t collision_bsp_test_vector_recursive(test_vector_data *data, int child_index, double t0, double t1);

/* Return is 8-bit: all 10 callers normalize the result with clrlwi rN,r3,24. */
uint8_t collision_bsp_test_vector(unsigned int flags, const struct collision_bsp *bsp,
                                  int16_t breakable_surface_count, const uint8_t *breakable_surface_flags,
                                  const real_point3d *point, const real_vector3d *vector, float maximum_t,
                                  collision_bsp_test_vector_result *result)
{
    test_vector_data data;
    double search_t = maximum_t;

    data.flags = flags;
    data.bsp = bsp;
    data.breakable_surface_count = breakable_surface_count;
    data.breakable_surface_flags = breakable_surface_flags;
    data.point = point;
    result->leaf_count = 0;
    data.vector = vector;
    data.result = result;
    data.last_leaf_index = -1;
    /* fsel f13,f0(maximum_t),f0,f1(0.0): clamp negatives to 0. The prior reconstruction had the
     * false branch as maximum_t; the binary selects 0.0. */
    result->t = maximum_t >= 0.0f ? maximum_t : 0.0f;
    data.last_contents = _contents_unknown;
    data.last_plane_index = -1;

    if ( maximum_t >= 0.0 )
    {
        if ( maximum_t > 1.0 )
            search_t = 1.0;
    }
    else
    {
        search_t = 0.0;
    }

    return collision_bsp_test_vector_recursive(&data, 0, 0.0, search_t);
}
