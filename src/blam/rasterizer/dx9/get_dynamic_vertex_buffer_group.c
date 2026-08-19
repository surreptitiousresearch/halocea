/* get_dynamic_vertex_buffer_group @0x837911B8 — returns the base of the dynamic_vertices group table.
 * `idx` is unused (confirmed via decompiler: the body is exactly `return &dynamic_vertices;`, with r3 never
 * read) — every caller apparently only needs the pool base, not a per-index lookup. */

#include <stdint.h>
#include "headers/dynamic_vertices.h"

dynamic_vertex_buffer_group * get_dynamic_vertex_buffer_group(int16_t idx)
{
    return dynamic_vertices.groups;
}
