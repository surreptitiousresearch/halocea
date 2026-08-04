#include "headers/d3d_boundary.h"

extern void D3DDevice_SetStreamSource(D3DDevice *pDevice, unsigned int StreamNumber, D3DVertexBuffer *pVertexBuffer, unsigned int OffsetInBytes, unsigned int StrideInBytes, unsigned long long PendingMask3);

/* DEVIATION: decompiler garbled; disasm: PendingMask3 = (1ULL<<63) >> (((95-StreamNumber)*0x5556u)>>16 + 32) */
void D3DDevice_SetStreamSource_Inline_2(D3DDevice *pDevice, unsigned int StreamNumber, D3DVertexBuffer *pStreamData, unsigned int OffsetInBytes, unsigned int Stride)
{
    unsigned int shift = (((95 - StreamNumber) * 0x5556u) >> 16) + 32;
    unsigned long long pending_mask = (1ULL << 63) >> shift;
    D3DDevice_SetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride, pending_mask);
}
