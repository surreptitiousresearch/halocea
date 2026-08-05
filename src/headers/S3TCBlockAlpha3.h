#pragma once

#include <stdint.h>
#include "S3TCBlockRGB.h"

/* S3TC (DXT5) compressed block: interpolated 3-bit alpha + RGB color block. */

typedef struct S3TCBlockAlpha3
{
    uint8_t alpha0;      /* 0x00 — interpolated 3-bit alpha endpoint 0 */
    uint8_t alpha1;      /* 0x01 — interpolated 3-bit alpha endpoint 1 */
    uint8_t alphabm[6];  /* 0x02 — 3-bit-per-pixel alpha index bitmap, 16 pixels */
    S3TCBlockRGB    rgb;         /* 0x08 */
} S3TCBlockAlpha3; /* 16 bytes */
