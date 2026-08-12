#pragma once

/* Xbox 360 D3D fill-mode enum (external library boundary type — declare, don't descend).
 * DB-verified: types_enum_values _D3DFILLMODE, all 4 members, DB order. The 360 numbering is
 * NOT the PC D3D9 numbering (PC: POINT=1, WIREFRAME=2, SOLID=3; here SOLID=0, POINT=1,
 * WIREFRAME=37/0x25 — a GPU register encoding, not an ordinal). */
typedef enum _D3DFILLMODE
{
    D3DFILL_SOLID       = 0,
    D3DFILL_POINT       = 1,
    D3DFILL_WIREFRAME   = 37,
    D3DFILL_FORCE_DWORD = 2147483647,
} _D3DFILLMODE; /* 4 bytes */
