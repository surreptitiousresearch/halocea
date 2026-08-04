/* collision_model — a model's collision geometry: damage/resistance data, pathfinding
 * bounds, and the node array against which rays are tested (664 bytes). */
#pragma once

#include "damage_resistance.h"
#include "real_rectangle3d.h"
#include "tag_block.h"

typedef struct collision_model
{
    damage_resistance resistance;           /* 0x000 */
    int               unused[4];            /* 0x258 */
    real_rectangle3d  pathfinding_box;      /* 0x268 */
    tag_block         pathfinding_spheres;  /* 0x280 — each 32 bytes */
    tag_block         nodes;                /* 0x28C — each 64 bytes */
} collision_model; /* 664 bytes */
