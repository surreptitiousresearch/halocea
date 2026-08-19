/* rasterizer_dx9_dynamic_vertex_buffer_set @0x83724048 — bind the dynamic vertex buffer (or none, if
 * vertex_buffer_index is 0) to a D3D stream, computing the same PendingMask3 bit as the
 * D3DDevice_SetStreamSource_Inline_N family: (1ULL<<63) >> (((95-stream_index)*0x5556)>>16 + 32).
 *
 * Deviation: the decompiler rendered the 64-bit shift as an int64 HIDWORD/LODWORD pun, and — in the
 * vertex_buffer_index==0 branch — mis-resolved the shift-amount constant as a coincidentally-adjacent global
 * address (`&player_ui_globals.edit_profile_data.edit_profile.variant + 1816`). Verified via disasm that both
 * branches compute the identical PendingMask3 formula (same as the established Inline_N siblings); the
 * "global" was never actually read. */

#include "headers/d3d_boundary.h"
#include "headers/blam_data_globals.h"

extern D3DVertexBuffer *hcex_dyn_vbuf_get(void);
extern void D3DDevice_SetStreamSource(D3DDevice *pDevice, unsigned int StreamNumber, D3DVertexBuffer *pVertexBuffer,
    unsigned int OffsetInBytes, unsigned int StrideInBytes, unsigned long long PendingMask3);

void rasterizer_dx9_dynamic_vertex_buffer_set(unsigned int vertex_buffer_index, unsigned int stream_index,
    unsigned int offset, unsigned int stride)
{
    D3DVertexBuffer *vertex_buffer = vertex_buffer_index ? hcex_dyn_vbuf_get() : 0;

    unsigned int shift = (((95 - stream_index) * 0x5556u) >> 16) + 32;
    unsigned long long pending_mask = (1ULL << 63) >> shift;

    D3DDevice_SetStreamSource(global_d3d_device, stream_index, vertex_buffer, offset, stride, pending_mask);
}
