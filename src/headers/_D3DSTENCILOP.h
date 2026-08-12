#pragma once

/* Xbox 360 D3D stencil-operation enum (external library boundary type — declare, don't
 * descend). DB-verified: types_enum_values _D3DSTENCILOP, all 9 members, DB order. Note the
 * 360 numbering (KEEP = 0) — PC D3D9 has D3DSTENCILOP_KEEP = 1. */
typedef enum _D3DSTENCILOP
{
    D3DSTENCILOP_KEEP        = 0,
    D3DSTENCILOP_ZERO        = 1,
    D3DSTENCILOP_REPLACE     = 2,
    D3DSTENCILOP_INCRSAT     = 3,
    D3DSTENCILOP_DECRSAT     = 4,
    D3DSTENCILOP_INVERT      = 5,
    D3DSTENCILOP_INCR        = 6,
    D3DSTENCILOP_DECR        = 7,
    D3DSTENCILOP_FORCE_DWORD = 2147483647,
} _D3DSTENCILOP; /* 4 bytes */
