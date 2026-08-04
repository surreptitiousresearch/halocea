#pragma once

/* D3D rectangle (external library boundary type). DB-verified (types_members _D3DRECT). */
typedef struct _D3DRECT
{
    int x1; /* 0x00 */
    int y1; /* 0x04 */
    int x2; /* 0x08 */
    int y2; /* 0x0C */
} _D3DRECT; /* 16 bytes */
