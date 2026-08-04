/* rasterizer_transparent_geometry_get_primary_vertex_type @0x837685C0 — the vertex format of a transparent
 * geometry group's primary buffer: a static vertex_buffers stream's type if present, else the dynamic
 * buffer's type, else -1 (no geometry). */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"

extern int16_t rasterizer_dynamic_vertices_get_type(int dynamic_vertex_buffer_index);

int16_t rasterizer_transparent_geometry_get_primary_vertex_type(const transparent_geometry_group *group)
{
    if ( group->vertex_buffers )
        return group->vertex_buffers->type;   /* int16_t field, callers extsh; lhz zero-ext cast redundant under 16-bit return */
    if ( group->dynamic_vertex_buffer_index != -1 )
        return rasterizer_dynamic_vertices_get_type(group->dynamic_vertex_buffer_index);
    return -1;
}
