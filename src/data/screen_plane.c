/* screen_plane @ 0x821265A4 — .rdata, real_plane3d (16 bytes). The view-space near clip plane
 * that src/portal_hull_from_points.c clips transformed portal polygons against before the
 * divide-by--z projection. Initializer reconstructed from the binary (big-endian words):
 *   +0x0000 0x00000000 -> n.i =  0.0f
 *   +0x0004 0x00000000 -> n.j =  0.0f
 *   +0x0008 0xBF800000 -> n.k = -1.0f
 *   +0x000C 0x3C23D70A -> d   =  0.01f  (near plane at z = -0.01 in view space)
 * The ledger extent of 20 is the next-named-symbol gap (sense_ray_length @0x821265B8); the four
 * trailing zero bytes are alignment, not a member — real_plane3d is a 16-byte type
 * (src/headers/real_plane3d.h, DB-verified).
 *
 * DEVIATION: src/portal_hull_from_points.c declares `extern real_plane3d screen_plane` without
 * const. The object is in .rdata (segment perm 4, read-only), so the original was
 * const-qualified; the consumer already passes it to a `const real_plane3d *` parameter. The
 * definition follows the segment and the consumer decl is untouched. */

#include "../headers/real_plane3d.h"

const real_plane3d screen_plane =
{
    { 0.0f, 0.0f, -1.0f },   /* n */
    0.01f,                   /* d */
};
