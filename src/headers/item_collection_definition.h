#pragma once
/* item_collection_definition — an item collection tag: a weighted list of item permutations from which a
 * random one is drawn. The permutations tag_block (at offset 0) holds the permutation entries; each
 * permutation entry is 84 bytes with the spawn weight at +32 and the item definition index at +48. */

#include <stdint.h>
#include "tag_block.h"

typedef struct item_collection_definition
{
    tag_block permutations; /* 0x00 */
    int16_t spawn_time;     /* 0x0C */
    int16_t pad;            /* 0x0E */
    int unused[19];         /* 0x10 */
} item_collection_definition; /* 92 bytes */
