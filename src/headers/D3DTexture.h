#pragma once

#include "D3DBaseTexture.h"

/* D3DTexture — Xbox 360 D3D SDK 2D texture (external library boundary type). DB: struct, size 52,
 * a single anonymous D3DBaseTexture at offset 0 (no fields of its own). Reproduced as a named
 * `base` member (C has no anonymous member of a named struct type). */
typedef struct D3DTexture
{
    D3DBaseTexture base; /* 0x00 (DB: anonymous D3DBaseTexture) */
} D3DTexture; /* 52 bytes */
