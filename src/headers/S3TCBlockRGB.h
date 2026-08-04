#pragma once

/* S3TC (DXT1) compressed RGB color block. */

typedef struct S3TCBlockRGB
{
    unsigned __int16 rgb0;   /* 0x00 — 565 color endpoint 0 */
    unsigned __int16 rgb1;   /* 0x02 — 565 color endpoint 1 */
    unsigned int     pixbm;  /* 0x04 — 2-bit-per-pixel index bitmap */
} S3TCBlockRGB; /* 8 bytes */
