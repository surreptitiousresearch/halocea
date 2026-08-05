/* geosphere_primitive_vertices @ 0x82128CE0 — .rdata, 72 bytes = 6 x real_point3d (12 each).
 * The six unit-octahedron vertices geosphere_new seeds a geosphere with before subdividing:
 * +z, +y, +x, -y, -x, -z. Initializer reconstructed from the binary (big-endian words), every
 * component 0x00000000 / 0x3F800000 / 0xBF800000. Consumer src/geosphere_new.c loops
 * vertex_index 0..5; DB applied type `const real_point3d geosphere_primitive_vertices[6]`. */

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
