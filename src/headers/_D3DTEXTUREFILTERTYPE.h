#pragma once

/* Xbox 360 D3D texture-filter enum (external library boundary type — declare, don't descend).
 * DB-verified: types_enum_values _D3DTEXTUREFILTERTYPE, all 5 members, DB order. The 360
 * numbering is NOT the PC D3D9 numbering (PC: NONE=0, POINT=1, LINEAR=2; here NONE=2,
 * POINT=0, LINEAR=1) — a literal 1 at a Min/MagFilter slot is LINEAR on this platform. */
typedef enum _D3DTEXTUREFILTERTYPE
{
    D3DTEXF_NONE        = 2,
    D3DTEXF_POINT       = 0,
    D3DTEXF_LINEAR      = 1,
    D3DTEXF_ANISOTROPIC = 4,
    D3DTEXF_FORCE_DWORD = 2147483647,
} _D3DTEXTUREFILTERTYPE; /* 4 bytes */
