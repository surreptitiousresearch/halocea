/* rasterizer_triangle_buffer_size @0x83807CF8 @0x83807CE0 — index count for a triangle buffer stream by type. */

#include <stdint.h>
#include "headers/triangle_buffer_type.h"

int rasterizer_triangle_buffer_size(int16_t type, int count)
{
    if ( type == _triangle_buffer_type_triangles )
        return 6 * count;
    if ( type == _triangle_buffer_type_precompiled_strip )
        return 2 * (count + 2);
    return 0;
}
