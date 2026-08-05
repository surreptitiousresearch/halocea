/* geosphere_primitive_triangles @ 0x82128D28 — .rdata, 48 bytes = 8 x 3 x int16_t.
 * The eight faces of the unit octahedron in geosphere_primitive_vertices: the first four fan
 * around vertex 0 (+z), the last four around vertex 5 (-z). geosphere_new passes each triple to
 * subdivide_triangle. Initializer reconstructed from the binary (big-endian 16-bit halves; the
 * pairs straddle word boundaries, e.g. 0x00000001 0x00020000 -> {0,1,2}).
 * DB applied type: `const __int16 geosphere_primitive_triangles[8][3]`. */

#include <stdint.h>

const int16_t geosphere_primitive_triangles[8][3] =
{
    { 0, 1, 2 },   /* +0x00 */
    { 0, 2, 3 },   /* +0x06 */
    { 0, 3, 4 },   /* +0x0C */
    { 0, 4, 1 },   /* +0x12 */
    { 5, 1, 4 },   /* +0x18 */
    { 5, 4, 3 },   /* +0x1E */
    { 5, 3, 2 },   /* +0x24 */
    { 5, 2, 1 },   /* +0x2A */
};
