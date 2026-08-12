#pragma once

/* Xbox 360 D3D cull-mode enum (external library boundary type — declare, don't descend).
 * DB-verified: types_enum_values _D3DCULL, all 4 members, DB order. Note the 360 numbering
 * (CW = 2, CCW = 6) — not a 0/1/2 ordinal. */
typedef enum _D3DCULL
{
    D3DCULL_NONE        = 0,
    D3DCULL_CW          = 2,
    D3DCULL_CCW         = 6,
    D3DCULL_FORCE_DWORD = 2147483647,
} _D3DCULL; /* 4 bytes */
