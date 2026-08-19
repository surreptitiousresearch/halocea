/* rasterizer_dynamic_geometry_begin @0x837912A0 — resets the per-frame dynamic vertex and triangle
 * pools: clears every dynamic vertex group's vertex count and both buffer counts. */

#include "headers/dynamic_vertices.h"
#include "headers/dynamic_triangles.h"

void rasterizer_dynamic_geometry_begin(void)
{
    for (int group = 0; group < 18; ++group)
        dynamic_vertices.groups[group].vertex_count = 0;
    dynamic_vertices.buffer_count = 0;

    dynamic_triangles.triangle_count = 0;
    dynamic_triangles.buffer_count = 0;
}
