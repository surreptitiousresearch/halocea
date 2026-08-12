#pragma once

/* Xbox 360 D3D comparison-function enum (external library boundary type — declare, don't
 * descend). DB-verified: types_enum_values _D3DCMPFUNC, all 9 members, DB order. Note the 360
 * numbering (NEVER = 0) — PC D3D9 has D3DCMP_NEVER = 1. */
typedef enum _D3DCMPFUNC
{
    D3DCMP_NEVER        = 0,
    D3DCMP_LESS         = 1,
    D3DCMP_EQUAL        = 2,
    D3DCMP_LESSEQUAL    = 3,
    D3DCMP_GREATER      = 4,
    D3DCMP_NOTEQUAL     = 5,
    D3DCMP_GREATEREQUAL = 6,
    D3DCMP_ALWAYS       = 7,
    D3DCMP_FORCE_DWORD  = 2147483647,
} _D3DCMPFUNC; /* 4 bytes */
