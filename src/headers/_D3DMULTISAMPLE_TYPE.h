#pragma once

/* Xbox 360 D3D multisample-type enum (external library boundary type). DB-verified
 * (types_enum_values _D3DMULTISAMPLE_TYPE). 4-byte SDK enum. */
typedef enum _D3DMULTISAMPLE_TYPE
{
    D3DMULTISAMPLE_NONE        = 0,
    D3DMULTISAMPLE_2_SAMPLES   = 1,
    D3DMULTISAMPLE_4_SAMPLES   = 2,
    D3DMULTISAMPLE_FORCE_DWORD = 2147483647,
} _D3DMULTISAMPLE_TYPE; /* 4 bytes */
