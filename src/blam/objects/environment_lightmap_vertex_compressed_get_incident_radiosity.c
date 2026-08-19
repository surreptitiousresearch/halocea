/* environment_lightmap_vertex_compressed_get_incident_radiosity @0x837E9838 — unpack a compressed lightmap
 * vertex's incident-radiosity direction. Identical packing to environment_vertex_compressed_get_normal: one
 * 32-bit word with three signed fixed-point fields (11 bits x, 11 bits y, 10 bits z), each mapped to ~[-1, 1].
 *
 * DEVIATION: the decompiler failed register allocation here ("local variable allocation has failed"); this is
 * reconstructed from the disassembly. */

#include "headers/environment_lightmap_vertex_compressed.h"
#include "headers/real_vector3d.h"

void environment_lightmap_vertex_compressed_get_incident_radiosity(
        const environment_lightmap_vertex_compressed *vertex, real_vector3d *normal)
{
    unsigned int packed = vertex->incident_radiosity;
    normal->n[0] = (((float)(int)(packed << 21) * 0.00000095367432f) + 1.0f) * 0.00048851978f;       /* 11-bit x */
    normal->n[1] = (((float)(int)((packed >> 11) << 21) * 0.00000095367432f) + 1.0f) * 0.00048851978f; /* 11-bit y */
    normal->n[2] = (((float)(int)(packed & 0xFFC00000) * 0.00000047683716f) + 1.0f) * 0.00097751711f;  /* 10-bit z */
}
