#pragma once
/* structure_lightmap — one lightmap page of a structure BSP (structure_bsp.lightmaps element,
 * 32-byte stride). Holds the lightmap bitmap index within the BSP's lightmap_group, plus the
 * tag_block of structure_material entries actually rendered for that page (count + address). */

#include <stdint.h>
#include "tag_block.h"

typedef struct structure_lightmap
{
    int16_t          bitmap_index;   /* 0x00 */
    uint16_t pad;            /* 0x02 */
    int              unused[4];      /* 0x04 */
    tag_block        materials;      /* 0x14 */
} structure_lightmap;               /* 32 bytes */
