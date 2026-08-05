#pragma once
/* structure_lens_flare_marker — one lens-flare marker in a structure BSP (16 bytes; DB-verified). */

#include <stdint.h>
#include "real_point3d.h"

typedef struct structure_lens_flare_marker
{
    real_point3d    position;           /* 0x00 */
    char            i_direction;        /* 0x0C */
    char            j_direction;        /* 0x0D */
    char            k_direction;        /* 0x0E */
    uint8_t lens_flare_index;   /* 0x0F */
} structure_lens_flare_marker;          /* 16 bytes */
