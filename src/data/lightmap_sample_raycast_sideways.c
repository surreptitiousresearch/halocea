/* lightmap_sample_raycast_sideways @ 0x82114148 (.rdata, 48 bytes)
 * DB applied_types: const real_vector3d lightmap_sample_raycast_sideways[4];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   [ 0] +0x00 n                          = 0xC12000000000000000000000
 *        +0x00 __s1                       = 0xC12000000000000000000000
 *   [ 1] +0x00 n                          = 0x412000000000000000000000
 *        +0x00 __s1                       = 0x412000000000000000000000
 *   [ 2] +0x00 n                          = 0x00000000C120000000000000
 *        +0x00 __s1                       = 0x00000000C120000000000000
 *   [ 3] +0x00 n                          = 0x000000004120000000000000
 *        +0x00 __s1                       = 0x000000004120000000000000
 * .rdata, 48 bytes = 4 x real_vector3d (12 each).
 * The four 10-world-unit horizontal probes lights_distant_lighting_at_point walks (raycast_count = 4)
 * when flag bit 0 is set, in place of the single downward lightmap_sample_raycast_down probe.
 * Initializer reconstructed from the binary (big-endian): -x, +x, -y, +y, magnitude 10
 * (0xC1200000 / 0x41200000). DB applied type: `const real_vector3d [4]`.
 */
#include "../headers/real_vector3d.h"

const real_vector3d lightmap_sample_raycast_sideways[4] =
{
    { { -10.0f,   0.0f,  0.0f } },   /* +0x00 */
    { {  10.0f,   0.0f,  0.0f } },   /* +0x0C */
    { {   0.0f, -10.0f,  0.0f } },   /* +0x18 */
    { {   0.0f,  10.0f,  0.0f } },   /* +0x24 */
};
