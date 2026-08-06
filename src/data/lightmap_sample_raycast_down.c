/* lightmap_sample_raycast_down @ 0x8211413C (.rdata, 12 bytes)
 * DB applied_types: real_vector3d lightmap_sample_raycast_down;
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x00 n                          = 0x0000000000000000C1200000
 *   +0x00 __s1                       = 0x0000000000000000C1200000
 * .rdata, 12 bytes = exactly sizeof(real_vector3d),
 * i.e. a scalar, not an array. The 10-world-unit downward probe passed to structure_test_vector
 * when a point's lightmap sample has to be found on the surface beneath it.
 * Initializer reconstructed from the binary (big-endian): 0x00000000, 0x00000000, 0xC1200000 = -10.
 * Scalar shape confirmed by all three consumers (src/light_particle.c,
 * src/lights_distant_lighting_at_point.c, src/lights_illumination_at_point.c), which all take its
 * address, and by the DB applied type `real_vector3d lightmap_sample_raycast_down`.
 */
#include "../headers/real_vector3d.h"

const real_vector3d lightmap_sample_raycast_down = { { 0.0f, 0.0f, -10.0f } };
