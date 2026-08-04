/* rasterizer_vertex_buffer_new @0x83807C78 — allocate and populate a hardware vertex buffer. `vertices2` is
 * unused (confirmed via disasm_range(0x83807C78,0x83807CE0), matching the decompiler); the size query call
 * to rasterizer_geometry_get_vertex_size is also made purely for its side effect, its result discarded. */

#include <stdint.h>
#include "headers/vertex_buffer.h"
#include "headers/blam_data_globals.h"

extern int16_t rasterizer_geometry_get_vertex_size(int16_t type);
extern void *hcex_stat_vbuf_create(int size);
extern int hcex_stat_vbuf_write(void *static_vbuf, const void *src, int size);

uint8_t rasterizer_vertex_buffer_new(vertex_buffer *vertex_buffer, int16_t type, int count, const void *vertices, const void *vertices2, int buffer_size)
{
    rasterizer_geometry_get_vertex_size(type);

    if ( global_d3d_device )
    {
        void *hardware_format = hcex_stat_vbuf_create(buffer_size);

        if ( vertices )
        {
            vertex_buffer->count = count;
            vertex_buffer->offset = hcex_stat_vbuf_write(hardware_format, vertices, buffer_size);
            vertex_buffer->type = type;
            vertex_buffer->hardware_format = hardware_format;
        }
    }
    return 1;
}
