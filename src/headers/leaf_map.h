#pragma once
/* leaf_map — runtime leaf/portal visibility map built for a structure BSP. Layout from the database. */

#include "tag_block.h"

struct bsp3d;

typedef struct leaf_map
{
    const struct bsp3d *bsp;     /* 0x00 */
    tag_block           leaves;  /* 0x04 */
    tag_block           portals; /* 0x10 */
} leaf_map;                      /* 0x1C */

/* leaf_map_globals_s (no DB type; reconstruction) lives in its own header so this file stays a
   pure DB-typed leaf_map definition; included here so existing consumers keep seeing both. */
#include "leaf_map_globals.h"
