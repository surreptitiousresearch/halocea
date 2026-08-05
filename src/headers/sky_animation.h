#pragma once
#include <stdint.h>
/* sky_animation — one entry in a sky tag's animations tag_block (36 bytes). DB-verified layout
 * (types_members sky_animation): the graph animation index and its playback period. */

typedef struct sky_animation
{
    int16_t          animation_index;   /* 0x00 */
    uint16_t pad;               /* 0x02 */
    float            period;            /* 0x04 — playback speed/period */
    int              unused[7];         /* 0x08 */
} sky_animation;                        /* 36 bytes */
