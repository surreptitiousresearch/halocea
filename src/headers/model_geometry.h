#pragma once
/* model_geometry — one entry of model->geometries: a tag_block of render parts. Layout from the
 * database (48 bytes). */

#include "tag_block.h"

typedef struct model_geometry
{
    unsigned int flags;      /* 0x00 */
    int          unused[8];  /* 0x04 */
    tag_block    parts;      /* 0x24 — gbxmodel_geometry_part[], 132 bytes each */
} model_geometry;             /* 0x30 (48 bytes) */
