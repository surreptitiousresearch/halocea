#pragma once
/* structure_leaf — a leaf of the structure BSP's leaf array (DB-verified, 16 bytes). */

#include <stdint.h>
#include "byte_rectangle3d.h"

typedef struct structure_leaf
{
    byte_rectangle3d bounds;            /* 0x00 */
    uint16_t pad;               /* 0x06 */
    int16_t cluster_index;              /* 0x08 */
    int16_t surface_reference_count;    /* 0x0A */
    int first_surface_reference_index;  /* 0x0C */
} structure_leaf;
