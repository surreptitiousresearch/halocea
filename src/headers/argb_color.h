#pragma once
/* argb_color — packed ARGB color, 16 bits per channel (8 bytes, DB types_members-confirmed).
 * DB union: raw n[4] view + two anonymous struct views ($-names kept verbatim; $ in
 * identifiers is an MSVC extension). */

#include <stdint.h>
#include "rgb_color.h"

/* DB struct _3DC870F3D1E824F33054915D98724D0C — alpha + rgb_color view. */
typedef struct _3DC870F3D1E824F33054915D98724D0C
{
    uint16_t alpha; /* 0x00 */
    rgb_color        rgb;   /* 0x02 */
} _3DC870F3D1E824F33054915D98724D0C;

/* DB struct _E7154C7975606A4346E79E85DBD1FA5A — per-channel view. */
typedef struct _E7154C7975606A4346E79E85DBD1FA5A
{
    uint8_t  gap0[2]; /* 0x00 — DB _BYTE[2] */
    uint16_t red;     /* 0x02 */
    uint16_t green;   /* 0x04 */
    uint16_t blue;    /* 0x06 */
} _E7154C7975606A4346E79E85DBD1FA5A;

typedef union argb_color
{
    uint16_t n[4];                            /* 0x00 */
    struct _3DC870F3D1E824F33054915D98724D0C __s1;    /* 0x00 — alpha + rgb view */
    struct _E7154C7975606A4346E79E85DBD1FA5A __s2;    /* 0x00 — channel view */
} argb_color; /* 8 bytes */
