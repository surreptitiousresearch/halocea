#pragma once

#include "real_rgb_color.h"

/* real_argb_color — ARGB color (16 bytes). DB union: float n[4] view, an
 * {alpha, real_rgb_color rgb} view, and a byte-offset {red,green,blue}
 * channel view ($-names kept verbatim; $ in identifiers is an MSVC
 * extension). */

/* DB struct _4B4036038E783B5674E58077268CAECC — alpha + rgb view. */
typedef struct _4B4036038E783B5674E58077268CAECC
{
    float          alpha; /* 0x00 */
    real_rgb_color rgb;   /* 0x04 */
} _4B4036038E783B5674E58077268CAECC;

/* DB struct _A1D52E832813CCBC7C7218CE9905C4B4 — per-channel float view. */
typedef struct _A1D52E832813CCBC7C7218CE9905C4B4
{
    unsigned __int8 gap0[4]; /* 0x00 — DB _BYTE[4] */
    float           red;     /* 0x04 */
    float           green;   /* 0x08 */
    float           blue;    /* 0x0C */
} _A1D52E832813CCBC7C7218CE9905C4B4;

typedef union real_argb_color
{
    float n[4];                                    /* 0x00 */
    struct _4B4036038E783B5674E58077268CAECC __s1; /* 0x00 — alpha + rgb view */
    struct _A1D52E832813CCBC7C7218CE9905C4B4 __s2; /* 0x00 — channel view */
    struct { float alpha; real_rgb_color rgb; };   /* 0x00 — direct alpha/rgb alias (same layout) */
} real_argb_color; /* 16 bytes */
