#pragma once

/* Xbox 360 D3D sampler-state selector enum (external library boundary type — declare, don't
 * descend). DB-verified: types_enum_values _D3DSAMPLERSTATETYPE, all 22 members, DB order.
 * The 360 numbering is a GPU register stride (each selector 4 apart), not the PC D3D9
 * 1..13 ordinals. */
typedef enum _D3DSAMPLERSTATETYPE
{
    D3DSAMP_ADDRESSU              = 0,
    D3DSAMP_ADDRESSV              = 4,
    D3DSAMP_ADDRESSW              = 8,
    D3DSAMP_BORDERCOLOR           = 12,
    D3DSAMP_MAGFILTER             = 16,
    D3DSAMP_MINFILTER             = 20,
    D3DSAMP_MIPFILTER             = 24,
    D3DSAMP_MIPMAPLODBIAS         = 28,
    D3DSAMP_MAXMIPLEVEL           = 32,
    D3DSAMP_MAXANISOTROPY         = 36,
    D3DSAMP_MAGFILTERZ            = 40,
    D3DSAMP_MINFILTERZ            = 44,
    D3DSAMP_SEPARATEZFILTERENABLE = 48,
    D3DSAMP_MINMIPLEVEL           = 52,
    D3DSAMP_TRILINEARTHRESHOLD    = 56,
    D3DSAMP_ANISOTROPYBIAS        = 60,
    D3DSAMP_HGRADIENTEXPBIAS      = 64,
    D3DSAMP_VGRADIENTEXPBIAS      = 68,
    D3DSAMP_WHITEBORDERCOLORW     = 72,
    D3DSAMP_POINTBORDERENABLE     = 76,
    D3DSAMP_MAX                   = 80,
    D3DSAMP_FORCE_DWORD           = 2147483647,
} _D3DSAMPLERSTATETYPE; /* 4 bytes */
