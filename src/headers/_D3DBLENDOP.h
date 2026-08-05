#pragma once

/* Xbox 360 D3D blend-operation enum (external library boundary type — declare, don't descend).
 * DB-verified: types_enum_values _D3DBLENDOP, all 6 members, DB order.
 *
 * As with _D3DBLEND the Xbox 360 numbering differs from PC D3D9 (PC: ADD = 1 .. MAX = 5).
 * blendop_table decodes correctly only here: framebuffer blend function 4 (reverse_subtract)
 * selects 4, 5 (min) selects 2 and 6 (max) selects 3. */
typedef enum _D3DBLENDOP
{
    D3DBLENDOP_ADD         = 0,
    D3DBLENDOP_SUBTRACT    = 1,
    D3DBLENDOP_MIN         = 2,
    D3DBLENDOP_MAX         = 3,
    D3DBLENDOP_REVSUBTRACT = 4,
    D3DBLENDOP_FORCE_DWORD = 2147483647,
} _D3DBLENDOP; /* 4 bytes */
