#pragma once
/* looping_sound_detail — one element of looping_sound_definition.details (104 bytes; DB-verified,
   matches C:\Users\user\Desktop\halo hdr reference\headers_ref\looping_sound_detail.h). */

#include "tag_reference.h"

typedef struct looping_sound_detail
{
    tag_reference sound;                /* 0x00 */
    float         period_lower_bound;   /* 0x10 */
    float         period_upper_bound;   /* 0x14 */
    float         gain;                 /* 0x18 */
    int           flags;                /* 0x1C */
    int           unused0[12];          /* 0x20 */
    float         theta_lower_bound;    /* 0x50 */
    float         theta_upper_bound;    /* 0x54 */
    float         phi_lower_bound;      /* 0x58 */
    float         phi_upper_bound;      /* 0x5C */
    float         distance_lower_bound; /* 0x60 */
    float         distance_upper_bound; /* 0x64 */
} looping_sound_detail;                 /* 104 bytes */
