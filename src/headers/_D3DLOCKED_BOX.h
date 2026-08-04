#pragma once

/* Xbox 360 D3D locked-box descriptor (external library boundary type). DB-verified
 * (types_members _D3DLOCKED_BOX). */
typedef struct _D3DLOCKED_BOX
{
    int   RowPitch;   /* 0x00 */
    int   SlicePitch; /* 0x04 */
    void *pBits;      /* 0x08 */
} _D3DLOCKED_BOX; /* 12 bytes */
