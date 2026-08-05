#pragma once
/* line_of_fire_pill — one friendly collision volume tested when checking an actor's line of fire.
 * Either a sphere (spherical != 0, using base + width) or a capsule (base + directed_height + width).
 * DB layout (PDB ground truth, 40 bytes). */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct line_of_fire_pill
{
    uint8_t spherical;       /* 0x00 */
    char            _pad01[3];       /* 0x01 */
    real_point3d    base;            /* 0x04 */
    real_vector3d   directed_height; /* 0x10 */
    int             prop_index;      /* 0x1C */
    int             unit_index;      /* 0x20 */
    float           width;           /* 0x24 */
} line_of_fire_pill;                 /* 40 bytes */
