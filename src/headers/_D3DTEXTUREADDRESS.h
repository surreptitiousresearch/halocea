#pragma once

/* Xbox 360 D3D texture-address-mode enum (external library boundary type — declare, don't
 * descend). DB-verified: types_enum_values _D3DTEXTUREADDRESS, all 9 members, DB order.
 *
 * Xbox 360 numbering (PC D3D9 starts at WRAP = 1). Pinned inside the corpus by the
 * u/v-clamped arms of rasterizer_dx9_transparent_chicago_draw, which pass a literal 2 for a
 * clamped map. */
typedef enum _D3DTEXTUREADDRESS
{
    D3DTADDRESS_WRAP                   = 0,
    D3DTADDRESS_MIRROR                 = 1,
    D3DTADDRESS_CLAMP                  = 2,
    D3DTADDRESS_MIRRORONCE             = 3,
    D3DTADDRESS_BORDER_HALF            = 4,
    D3DTADDRESS_MIRRORONCE_BORDER_HALF = 5,
    D3DTADDRESS_BORDER                 = 6,
    D3DTADDRESS_MIRRORONCE_BORDER      = 7,
    D3DTADDRESS_FORCE_DWORD            = 2147483647,
} _D3DTEXTUREADDRESS; /* 4 bytes */
