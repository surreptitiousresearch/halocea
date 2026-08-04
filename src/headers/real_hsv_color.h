#pragma once

/* real_hsv_color — HSV color, three floats: hue (normalized 0..1, wraps),
 * saturation, value (12 bytes). DB union: flat n[3] view plus the named
 * hue/saturation/value anonymous struct view ($-name kept verbatim; $ in
 * identifiers is an MSVC extension). */

/* DB struct _90B0CF3FB4D6E3DF758C531B060A7611 — hue/saturation/value view. */
typedef struct _90B0CF3FB4D6E3DF758C531B060A7611
{
    float hue;        /* 0x00 */
    float saturation; /* 0x04 */
    float value;      /* 0x08 */
} _90B0CF3FB4D6E3DF758C531B060A7611;

typedef union real_hsv_color
{
    float n[3];                                    /* 0x00 — [0]=hue, [1]=saturation, [2]=value */
    struct _90B0CF3FB4D6E3DF758C531B060A7611 __s1; /* 0x00 — named view */
} real_hsv_color; /* 12 bytes */
