#pragma once

/* Xbox 360 D3D primitive-type enum (external library boundary type). DB-verified
 * (types_enum_values _D3DPRIMITIVETYPE). */
typedef enum _D3DPRIMITIVETYPE
{
    D3DPT_POINTLIST     = 1,
    D3DPT_LINELIST      = 2,
    D3DPT_LINESTRIP     = 3,
    D3DPT_TRIANGLELIST  = 4,
    D3DPT_TRIANGLEFAN   = 5,
    D3DPT_TRIANGLESTRIP = 6,
    D3DPT_RECTLIST      = 8,
    D3DPT_QUADLIST      = 13,
    D3DPT_FORCE_DWORD   = 2147483647,
} _D3DPRIMITIVETYPE; /* 4 bytes */
