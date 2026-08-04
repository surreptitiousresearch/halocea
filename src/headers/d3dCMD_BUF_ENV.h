#pragma once
/* d3dCMD_BUF_ENV — D3D command-buffer environment: vidCMD_BUF_ENV base plus tiling/render-target
 * state (DB types_members d3dCMD_BUF_ENV, 64 bytes). */

#include "vidCMD_BUF_ENV.h"
#include "_D3DVIEWPORT9.h"

typedef struct _D3DRECT   _D3DRECT;
typedef struct D3DSurface D3DSurface;

typedef struct d3dCMD_BUF_ENV
{
    vidCMD_BUF_ENV base;        /* 0x00 — DB unnamed base class vidCMD_BUF_ENV */
    _D3DRECT      *tilingRects; /* 0x14 */
    unsigned int   tileCount;   /* 0x18 */
    D3DSurface    *colorRT;     /* 0x1C */
    D3DSurface    *depthRT;     /* 0x20 */
    _D3DVIEWPORT9  vp;          /* 0x24 */
    bool           curScenario; /* 0x3C */
    bool           colorRTgamma;/* 0x3D */
    unsigned char  _pad3E[2];   /* 0x3E */
} d3dCMD_BUF_ENV; /* 64 bytes */
