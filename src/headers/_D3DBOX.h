#pragma once

/* Xbox 360 D3D volume-box descriptor (external library boundary type). DB-verified
 * (types_members _D3DBOX). The DB carries both SDK spellings of this one struct — the
 * `typedef struct _D3DBOX {...} D3DBOX;` tag and its typedef name are separate type
 * ordinals (7187 / 48991, identical layout); D3DBOX.h holds the other one, and the D3D
 * wrapper prototypes in applied_types use this `_`-prefixed spelling. */
typedef struct _D3DBOX
{
    unsigned int Left;   /* 0x00 */
    unsigned int Top;    /* 0x04 */
    unsigned int Right;  /* 0x08 */
    unsigned int Bottom; /* 0x0C */
    unsigned int Front;  /* 0x10 */
    unsigned int Back;   /* 0x14 */
} _D3DBOX; /* 24 bytes */
