#pragma once
/* model_region_permutation — one selectable permutation of a model region. 88 bytes; layout from the
 * database. flags bit 0x1 = "cannot be chosen randomly"; variant_number < 100 are real variants. */

#include "tag_block.h"

typedef struct model_region_permutation
{
    char             name[32];           /* 0x00 */
    unsigned int     flags;              /* 0x20 */
    __int16          variant_number;     /* 0x24 */
    unsigned __int16 pad;                /* 0x26 */
    int              unused[6];          /* 0x28 */
    __int16          geometry_indices[5];/* 0x40 */
    unsigned __int16 pad2;               /* 0x4A */
    tag_block        markers;            /* 0x4C */
} model_region_permutation;             /* 88 bytes */
