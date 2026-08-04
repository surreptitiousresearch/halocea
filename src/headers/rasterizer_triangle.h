#pragma once
/* rasterizer_triangle — one dynamic-geometry triangle: three packed vertex indices. Layout from the
 * database (6 bytes). */

typedef struct rasterizer_triangle
{
    unsigned __int16 vertex_indices[3];   /* 0x00 */
} rasterizer_triangle;                     /* 0x06 */
