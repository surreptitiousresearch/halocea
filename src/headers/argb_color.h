#pragma once
/* argb_color — packed ARGB color, 16 bits per channel (8 bytes, DB types_members-confirmed).
 * DB union: raw n[4] view + two anonymous struct views ($-names kept verbatim; $ in
 * identifiers is an MSVC extension). */

#include "rgb_color.h"

/* DB struct _3DC870F3D1E824F33054915D98724D0C — alpha + rgb_color view. */
typedef struct _3DC870F3D1E824F33054915D98724D0C
{
    unsigned __int16 alpha; /* 0x00 */
    rgb_color        rgb;   /* 0x02 */
} _3DC870F3D1E824F33054915D98724D0C;

/* DB struct _E7154C7975606A4346E79E85DBD1FA5A — per-channel view. */
typedef struct _E7154C7975606A4346E79E85DBD1FA5A
{
    unsigned __int8  gap0[2]; /* 0x00 — DB _BYTE[2] */
    unsigned __int16 red;     /* 0x02 */
    unsigned __int16 green;   /* 0x04 */
    unsigned __int16 blue;    /* 0x06 */
} _E7154C7975606A4346E79E85DBD1FA5A;

typedef union argb_color
{
    unsigned __int16 n[4];                            /* 0x00 */
    struct _3DC870F3D1E824F33054915D98724D0C __s1;    /* 0x00 — alpha + rgb view */
    struct _E7154C7975606A4346E79E85DBD1FA5A __s2;    /* 0x00 — channel view */
} argb_color; /* 8 bytes */
