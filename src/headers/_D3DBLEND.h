#pragma once

/* Xbox 360 D3D blend-factor enum (external library boundary type — declare, don't descend).
 * DB-verified: types_enum_values _D3DBLEND, all 16 members, DB order.
 *
 * The Xbox 360 numbering is NOT the PC D3D9 numbering (PC has D3DBLEND_ZERO = 1). The shipped
 * srcblend_table / destblend_table words only decode to sane blend equations under this
 * numbering, and rasterizer_set_framebuffer_blend_function's broken-min/max fallback passes a
 * literal 1 where an additive blend is wanted, which pins D3DBLEND_ONE == 1. */
typedef enum _D3DBLEND
{
    D3DBLEND_ZERO             = 0,
    D3DBLEND_ONE              = 1,
    D3DBLEND_SRCCOLOR         = 4,
    D3DBLEND_INVSRCCOLOR      = 5,
    D3DBLEND_SRCALPHA         = 6,
    D3DBLEND_INVSRCALPHA      = 7,
    D3DBLEND_DESTCOLOR        = 8,
    D3DBLEND_INVDESTCOLOR     = 9,
    D3DBLEND_DESTALPHA        = 10,
    D3DBLEND_INVDESTALPHA     = 11,
    D3DBLEND_BLENDFACTOR      = 12,
    D3DBLEND_INVBLENDFACTOR   = 13,
    D3DBLEND_CONSTANTALPHA    = 14,
    D3DBLEND_INVCONSTANTALPHA = 15,
    D3DBLEND_SRCALPHASAT      = 16,
    D3DBLEND_FORCE_DWORD      = 2147483647,
} _D3DBLEND; /* 4 bytes */
