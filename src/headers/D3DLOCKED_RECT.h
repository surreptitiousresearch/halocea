#pragma once

/* Xbox 360 D3D locked-rect descriptor (external library boundary type). DB-verified. */
typedef struct D3DLOCKED_RECT {
    int   Pitch; /* 0x00 */
    void *pBits; /* 0x04 */
} D3DLOCKED_RECT; /* 8 bytes */
