#pragma once
#include <stdint.h>
/* collision_leaf — a leaf of a collision BSP's leaf array (DB-verified, 8 bytes). */

typedef struct collision_leaf
{
    uint16_t flags;                 /* 0x00 */
    int16_t bsp2d_reference_count;          /* 0x02 */
    int first_bsp2d_reference_index;        /* 0x04 */
} collision_leaf;
