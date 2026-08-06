/* geosphere_primitive_vertices @ 0x82128CE0 (.rdata, 72 bytes)
 * DB applied_types: const real_point3d geosphere_primitive_vertices[6];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   [ 0] +0x00 n                          = 0x00000000000000003F800000
 *        +0x00 __s1                       = 0x00000000000000003F800000
 *        +0x00 __s2                       = 0x00000000000000003F800000
 *   [ 1] +0x00 n                          = 0x000000003F80000000000000
 *        +0x00 __s1                       = 0x000000003F80000000000000
 *        +0x00 __s2                       = 0x000000003F80000000000000
 *   [ 2] +0x00 n                          = 0x3F8000000000000000000000
 *        +0x00 __s1                       = 0x3F8000000000000000000000
 *        +0x00 __s2                       = 0x3F8000000000000000000000
 *   [ 3] +0x00 n                          = 0x00000000BF80000000000000
 *        +0x00 __s1                       = 0x00000000BF80000000000000
 *        +0x00 __s2                       = 0x00000000BF80000000000000
 *   [ 4] +0x00 n                          = 0xBF8000000000000000000000
 *        +0x00 __s1                       = 0xBF8000000000000000000000
 *        +0x00 __s2                       = 0xBF8000000000000000000000
 *   [ 5] +0x00 n                          = 0x0000000000000000BF800000
 *        +0x00 __s1                       = 0x0000000000000000BF800000
 *        +0x00 __s2                       = 0x0000000000000000BF800000
 * .rdata, 72 bytes = 6 x real_point3d (12 each).
 * The six unit-octahedron vertices geosphere_new seeds a geosphere with before subdividing:
 * +z, +y, +x, -y, -x, -z. Initializer reconstructed from the binary (big-endian words), every
 * component 0x00000000 / 0x3F800000 / 0xBF800000. Consumer src/geosphere_new.c loops
 * vertex_index 0..5; DB applied type `const real_point3d geosphere_primitive_vertices[6]`.
 */
#include "../headers/real_point3d.h"

const real_point3d geosphere_primitive_vertices[6] =
{
    { {  0.0f,  0.0f,  1.0f } },   /* [0] +0x00  +z */
    { {  0.0f,  1.0f,  0.0f } },   /* [1] +0x0C  +y */
    { {  1.0f,  0.0f,  0.0f } },   /* [2] +0x18  +x */
    { {  0.0f, -1.0f,  0.0f } },   /* [3] +0x24  -y */
    { { -1.0f,  0.0f,  0.0f } },   /* [4] +0x30  -x */
    { {  0.0f,  0.0f, -1.0f } },   /* [5] +0x3C  -z */
};
