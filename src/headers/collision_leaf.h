#pragma once
/* collision_leaf — a leaf of a collision BSP's leaf array (DB-verified, 8 bytes). */

typedef struct collision_leaf
{
    unsigned __int16 flags;                 /* 0x00 */
    __int16 bsp2d_reference_count;          /* 0x02 */
    int first_bsp2d_reference_index;        /* 0x04 */
} collision_leaf;
