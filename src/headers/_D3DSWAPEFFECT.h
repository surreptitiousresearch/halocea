#pragma once

/* Xbox 360 D3D swap-effect enum (external library boundary type). DB-verified
 * (types_enum_values _D3DSWAPEFFECT). 4-byte SDK enum. */
typedef enum _D3DSWAPEFFECT
{
    D3DSWAPEFFECT_DISCARD     = 1,
    D3DSWAPEFFECT_FLIP        = 2,
    D3DSWAPEFFECT_COPY        = 3,
    D3DSWAPEFFECT_FORCE_DWORD = 2147483647,
} _D3DSWAPEFFECT; /* 4 bytes */
