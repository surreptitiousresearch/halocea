#pragma once
/* structure_fog_plane — a fog plane in a structure BSP's fog_planes block (DB-verified, 32 bytes). */

#include "real_plane3d.h"
#include "tag_block.h"

typedef struct structure_fog_plane
{
    __int16 region_index;           /* 0x00 */
    __int16 runtime_material_type;  /* 0x02 */
    real_plane3d plane;             /* 0x04 */
    tag_block vertices;             /* 0x14 */
} structure_fog_plane;
