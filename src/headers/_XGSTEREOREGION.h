#pragma once

/* Xbox 360 XGraphics stereo region (external library boundary type).
 * DB-verified (types_members _XGSTEREOREGION). */
#include "_D3DRECT.h"
#include "_D3DPOINT.h"

typedef struct _XGSTEREOREGION
{
    _D3DRECT FrontBufferRect;    /* 0x00 */
    _D3DRECT BlankRectTop;       /* 0x10 */
    _D3DRECT BlankRectBottom;    /* 0x20 */
    _D3DRECT ResolveSourceRect;  /* 0x30 */
    _D3DPOINT ResolveDestPoint;  /* 0x40 */
    int ViewportYOffset;         /* 0x48 */
} _XGSTEREOREGION; /* 76 bytes */
