#pragma once
/* item_permutation_definition — one weighted entry of an item collection tag's permutations block.
 * Layout DB-verified against type 'item_permutation_definition' (84 bytes, 2026-07-14). */

#include "tag_reference.h"

typedef struct item_permutation_definition
{
    int           unused1[8]; /* 0x00 */
    float         weight;     /* 0x20 (32) — spawn weight */
    tag_reference item;       /* 0x24 (36) — item definition */
    int           unused2[8]; /* 0x34 (52) */
} item_permutation_definition; /* 84 bytes */
