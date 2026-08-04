#pragma once
/* model_marker — a named marker on a model tag, with its per-region/permutation
 * instances (DB-verified, 64 bytes). */

#include "tag_block.h"

typedef struct model_marker
{
    char name[32];          /* 0x00 */
    int unused[5];          /* 0x20 */
    tag_block instances;    /* 0x34: model_marker_instance[] */
} model_marker;
