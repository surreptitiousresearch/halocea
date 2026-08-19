/* _rasterizer_dynamic_triangles_new @0x837912F0 — reserve a new dynamic-triangle sub-buffer of `count`
 * triangles from the per-frame pool. Returns its buffer index, or -1 if the request is empty, would overflow
 * the 0x8000-triangle pool, or the 1023-buffer limit is reached (the overflow is warned about once). */

#include "headers/dynamic_triangles.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_limits.h"

extern dynamic_triangles_struct dynamic_triangles;

int _rasterizer_dynamic_triangles_new(int count)
{
    int result = -1;
    if (count > 0)
    {
        if (dynamic_triangles.triangle_count >= RASTERIZER_MAXIMUM_DYNAMIC_TRIANGLES - count || dynamic_triangles.buffer_count >= RASTERIZER_MAXIMUM_DYNAMIC_TRIANGLE_BUFFERS - 1)
        {
            if (!warned_4)
                warned_4 = 1;
        }
        else
        {
            result = dynamic_triangles.buffer_count;
            dynamic_triangles.buffers[dynamic_triangles.buffer_count].triangle_start_ofs =
                3 * dynamic_triangles.triangle_count;
            dynamic_triangles.buffers[dynamic_triangles.buffer_count].triangle_count = count;
            ++dynamic_triangles.buffer_count;
            dynamic_triangles.triangle_count += count;
        }
    }
    return result;
}
