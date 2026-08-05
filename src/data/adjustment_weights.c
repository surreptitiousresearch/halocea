/* adjustment_weights @ 0x82110C18 — .rdata, 108 bytes = 9 x real_point3d (12 each), no padding.
 * Search offsets (in the caller's local frame, scaled by real_matrix4x3::scale) tried in order when
 * player_teleport_internal has to move a unit out of a blocked exit: the +x axis, the two +/-y axes,
 * the two xy-diagonals, then the four (+x, +/-y, +/-z) body diagonals.
 * Initializer reconstructed from the binary (big-endian words), 0x3F3504F3 = 0.70710677 (1/sqrt 2),
 * 0x3F13CD3A = 0.57735026 (1/sqrt 3). DB applied type: `const real_point3d adjustment_weights[9]`. */

#include "../headers/real_point3d.h"

const real_point3d adjustment_weights[9] =
{
    { {  1.0f,         0.0f,        0.0f        } },   /* +0x00 */
    { {  0.0f,         1.0f,        0.0f        } },   /* +0x0C */
    { {  0.0f,        -1.0f,        0.0f        } },   /* +0x18 */
    { {  0.70710677f, -0.70710677f, 0.0f        } },   /* +0x24 */
    { {  0.70710677f,  0.70710677f, 0.0f        } },   /* +0x30 */
    { {  0.57735026f,  0.57735026f, 0.57735026f } },   /* +0x3C */
    { {  0.57735026f,  0.57735026f, -0.57735026f } },  /* +0x48 */
    { {  0.57735026f, -0.57735026f, 0.57735026f } },   /* +0x54 */
    { {  0.57735026f, -0.57735026f, -0.57735026f } },  /* +0x60 */
};
