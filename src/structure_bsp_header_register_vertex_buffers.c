/* structure_bsp_header_register_vertex_buffers @0x83754790 — tail-call forwarder
 * (`b structure_bsp_header_register_vertex_buffers_new`; confirmed via disasm, not a bare blr). The
 * "_new" implementation itself is not yet decompiled — declared extern with its own DB prototype and
 * called through with the same argument. */

#include "headers/cache_file_structure_bsp_header.h"

extern void structure_bsp_header_register_vertex_buffers_new(cache_file_structure_bsp_header *structure_bsp_header);

void structure_bsp_header_register_vertex_buffers(cache_file_structure_bsp_header *structure_bsp_header)
{
    structure_bsp_header_register_vertex_buffers_new(structure_bsp_header);
}
