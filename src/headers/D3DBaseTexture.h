#pragma once

#include "D3DResource.h"
#include "GPUTEXTURE_FETCH_CONSTANT.h"

/* D3DBaseTexture — Xbox 360 D3D SDK base-texture resource (external library boundary type).
 * DB-verified (types_members D3DBaseTexture): D3DResource base@0 (24B),
 * MipFlush@24, Format@28 (GPUTEXTURE_FETCH_CONSTANT). */
typedef struct D3DBaseTexture
{
    D3DResource               base;                 /* 0x00 D3DResource baseclass */
    unsigned int              MipFlush;             /* 0x18 */
    GPUTEXTURE_FETCH_CONSTANT Format;               /* 0x1C */
} D3DBaseTexture; /* 52 bytes */
