#pragma once

/* Xbox 360 D3D viewport (external library boundary type). DB-verified (types_members
 * _D3DVIEWPORT9). Distinct DB tag from the D3DVIEWPORT9 typedef sibling (same layout). */
typedef struct _D3DVIEWPORT9
{
    unsigned int X;      /* 0x00 */
    unsigned int Y;      /* 0x04 */
    unsigned int Width;  /* 0x08 */
    unsigned int Height; /* 0x0C */
    float        MinZ;   /* 0x10 */
    float        MaxZ;   /* 0x14 */
} _D3DVIEWPORT9; /* 24 bytes */
