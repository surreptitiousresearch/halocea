#pragma once

/* Xbox 360 D3D box descriptor (external library boundary type). DB-verified. */
typedef struct D3DBOX {
    unsigned int Left;   /* 0x00 */
    unsigned int Top;    /* 0x04 */
    unsigned int Right;  /* 0x08 */
    unsigned int Bottom; /* 0x0C */
    unsigned int Front;  /* 0x10 */
    unsigned int Back;   /* 0x14 */
} D3DBOX; /* 24 bytes */
