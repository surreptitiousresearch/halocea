/* environment_vertex_compressed_get_normal @0x837E9768 — unpack a compressed structure vertex normal. The
 * normal is stored as a single 32-bit word holding three signed fixed-point fields (11 bits x, 11 bits y,
 * 10 bits z). Each field is sign-extended (by shifting it up to the top of a 32-bit word), converted to float,
 * and mapped to roughly [-1, 1] via (raw*2 + 1)/(2^bits - 1).
 *
 * DEVIATION: the decompiler failed register allocation here ("local variable allocation has failed"); this is
 * reconstructed from the disassembly. The field math uses the exact shift/scale constants the binary uses. */

#include "headers/environment_vertex_compressed.h"
#include "headers/real_vector3d.h"

void environment_vertex_compressed_get_normal(const environment_vertex_compressed *vertex, real_vector3d *normal)
{
    unsigned int packed = vertex->normal;
    normal->n[0] = (((float)(int)(packed << 21) * 0.00000095367432f) + 1.0f) * 0.00048851978f;       /* 11-bit x */
    normal->n[1] = (((float)(int)((packed >> 11) << 21) * 0.00000095367432f) + 1.0f) * 0.00048851978f; /* 11-bit y */
    normal->n[2] = (((float)(int)(packed & 0xFFC00000) * 0.00000047683716f) + 1.0f) * 0.00097751711f;  /* 10-bit z */
}
