#pragma once
/* structure_fog_plane — a fog plane in a structure BSP's fog_planes block (DB-verified, 32 bytes). */

#include <stdint.h>
#include "real_plane3d.h"
#include "tag_block.h"

typedef struct structure_fog_plane
{
    int16_t region_index;           /* 0x00 */
    int16_t runtime_material_type;  /* 0x02 */
    real_plane3d plane;             /* 0x04 */
    tag_block vertices;             /* 0x14 */
} structure_fog_plane;
