#pragma once

#include "S3TCBlockRGB.h"

/* S3TC (DXT3) compressed block: explicit 4-bit alpha + RGB color block. */

typedef struct S3TCBlockAlpha4
{
    unsigned __int16 alphabm[4]; /* 0x00 — 4-bit-per-pixel alpha index bitmap, 4 rows */
    S3TCBlockRGB     rgb;        /* 0x08 */
} S3TCBlockAlpha4; /* 16 bytes */
