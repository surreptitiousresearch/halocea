/* rasterizer_dynamic_geometry_initialize @ 0x837911C8
   Computes the cumulative maximum vertex count for dynamic-vertex group 0 by
   summing per-usage capacities over the 18 dynamic-vertex usages. Reports
   failure if any nonzero-capacity usage lacks a backing vertex buffer.

   Note: the decompiler collapses all per-usage state onto groups[0]; the
   running total and the buffer-presence flag drive both the stored
   max_vertex_count and the success result. */

#include <stdint.h>

#include "headers/dynamic_vertices.h"
#include "headers/rasterizer_vertex_type.h"

/* uint8_t return (was int): the loop's success test byte-truncates (clrlwi r8,r3,24
   @83791284) and the caller does the same before testing (@8369BBD4) — the value is
   a byte-typed boolean, not a full-width int. */
uint8_t rasterizer_dynamic_geometry_initialize(void)
{
    uint8_t succeeded = 1;
    int running_total = 0;
    unsigned int has_vertex_buffer = dynamic_vertices.groups[0].vertex_buffer_index;

    for ( short usage = 0; usage < NUMBER_OF_RASTERIZER_VERTEX_TYPES; ++usage )
    {
        int usage_capacity;
        switch ( usage )
        {
            case _rasterizer_vertex_type_model_uncompressed: usage_capacity = 2048;   break;
            case _rasterizer_vertex_type_dynamic_unlit:
            case _rasterizer_vertex_type_model_processed:    usage_capacity = 0x2000; break;
            case _rasterizer_vertex_type_dynamic_lit:        usage_capacity = 2;      break;
            case _rasterizer_vertex_type_dynamic_screen:     usage_capacity = 0x4000; break;
            default:                                         usage_capacity = 0;      break;
        }
        running_total += usage_capacity;
        if ( usage_capacity > 0 && !has_vertex_buffer )
            succeeded = 0;

        dynamic_vertices.groups[0].max_vertex_count = running_total;
        has_vertex_buffer = 1;
        dynamic_vertices.groups[0].vertex_buffer_index = 1;

        if ( !succeeded ) /* byte-typed local models the clrlwi truncation directly */
            break;
    }
    return succeeded;
}
