/* rasterizer_triangle_buffer_new @0x83807D30 — allocate and populate a hardware index buffer for a stream
 * of triangle indices. `type` selects the index encoding: 0 = triangle list (3 indices/triangle), 1 = strip
 * (count+2 indices), else count is already the raw index count; every index is 2 bytes. */

#include <stdint.h>
#include "headers/triangle_buffer.h"
#include "headers/triangle_buffer_type.h"
#include "headers/blam_data_globals.h"

extern void *hcex_stat_ibuf_create(int size);
extern void hcex_stat_ibuf_write(void *static_ibuf, const void *src, int size);

uint8_t rasterizer_triangle_buffer_new(triangle_buffer *triangle_buffer, int16_t type, int count, const void *triangles)
{
    int index_count;

    if ( type == _triangle_buffer_type_triangles )
        index_count = 3 * count;
    else if ( type == _triangle_buffer_type_precompiled_strip )
        index_count = count + 2;
    else
        index_count = count;

    int byte_size = 2 * index_count;

    triangle_buffer->type = type;
    triangle_buffer->count = count;

    if ( global_d3d_device )
    {
        void *hardware_format = hcex_stat_ibuf_create(byte_size);

        hcex_stat_ibuf_write(hardware_format, triangles, byte_size);
        triangle_buffer->hardware_format = hardware_format;
    }

    return 1;
}
