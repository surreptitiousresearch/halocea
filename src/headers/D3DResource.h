#pragma once

/* D3DResource — Xbox 360 D3D SDK base resource (external library boundary type).
 * DB-verified (types_members D3DResource): 6x u32, 24 bytes. */
typedef struct D3DResource
{
    unsigned int Common;         /* 0x00 */
    unsigned int ReferenceCount; /* 0x04 */
    unsigned int Fence;          /* 0x08 */
    unsigned int ReadFence;      /* 0x0C */
    unsigned int Identifier;     /* 0x10 */
    unsigned int BaseFlush;      /* 0x14 */
} D3DResource; /* 24 bytes */
