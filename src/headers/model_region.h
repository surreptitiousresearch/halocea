#pragma once
/* model_region — one named region of a model (a set of selectable permutations). 76 bytes; layout from
 * the database. */

#include "tag_block.h"

typedef struct model_region
{
    char      name[32];      /* 0x00 */
    int       unused[8];     /* 0x20 */
    tag_block permutations;  /* 0x40 — 88-byte model_region_permutation records */
} model_region;             /* 76 bytes */
