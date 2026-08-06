/* global_projection3d_mappings @0x821145B4 — .rdata, 24 bytes; DB applied type
 * `const __int16[3][2][2]`, matching the canonical declaration in headers/projection3d_globals.h.
 * Indexed [projection_axis][projection_sign][component]: for each axis-aligned projection and each
 * sign orientation, the two source axis indices copied into the 2D result's x and y.
 * Big-endian halfword pairs in address order:
 *   +0x0000 {2,1}   axis 0 (x), sign 0 -> (z,y)
 *   +0x0004 {1,2}   axis 0 (x), sign 1 -> (y,z)
 *   +0x0008 {0,2}   axis 1 (y), sign 0 -> (x,z)
 *   +0x000C {2,0}   axis 1 (y), sign 1 -> (z,x)
 *   +0x0010 {1,0}   axis 2 (z), sign 0 -> (y,x)
 *   +0x0014 {0,1}   axis 2 (z), sign 1 -> (x,y)
 * The sign flips the pair so the 2D winding stays consistent with the plane normal's direction.
 *
 * Shape is proven, not assumed: obstacles_add_disc @0x8382FA0C..0x8382FA10 constant-folds
 * `global_projection3d_mappings[2][1][0..1]` to the object's +0x14/+0x16 halfwords. That is only
 * correct for an 8-byte outer stride (3x2x2); a [1][6][2] outer stride of 24 would put the same
 * expression 28 bytes past the object's base, outside it entirely.
 *
 * DEVIATION (resolved 2026-08-06, BACKLOG A8): the 21 declaring TUs used to disagree three ways —
 * 16 spelled it `const int16_t[1][6][2]` and indexed `[0][2*axis+sign][c]`, 4 spelled `[3][2][2]`,
 * 1 spelled `[][2]`. All three computed the same byte offsets (2*axis+sign is exactly the flattened
 * (axis, sign) pair index), so none miscompiled, but only [3][2][2] is the real shape and the flat
 * form indexed past the inner dimension. Every consumer now drops its local extern, includes
 * headers/blam_data_globals.h, and indexes [axis][sign][component]. */

#include <stdint.h>
#include "../headers/projection3d_globals.h"

const int16_t global_projection3d_mappings[3][2][2] =
{
    { { 2, 1 }, { 1, 2 } },   /* project along x */
    { { 0, 2 }, { 2, 0 } },   /* project along y */
    { { 1, 0 }, { 0, 1 } }    /* project along z */
};
