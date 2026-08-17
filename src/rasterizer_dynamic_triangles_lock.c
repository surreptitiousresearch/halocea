/* rasterizer_dynamic_triangles_lock @0x836A5DD8 */
/* rasterizer_dynamic_triangles_lock 0x836A5DD8 — tail-call thunk to _rasterizer_dynamic_triangles_lock. */

#include "headers/rasterizer_triangle.h"

extern rasterizer_triangle *_rasterizer_dynamic_triangles_lock(int dynamic_triangle_buffer_index);

rasterizer_triangle *rasterizer_dynamic_triangles_lock(int dynamic_triangle_buffer_index)
{
    return _rasterizer_dynamic_triangles_lock(dynamic_triangle_buffer_index);
}
