#pragma once
/* material_effect_material — one per-material record inside a material_effect block
 * (48 bytes, DB types_members-confirmed). */

#include "tag_reference.h"

typedef struct material_effect_material
{
    tag_reference effect; /* 0x00 */
    tag_reference sound;  /* 0x10 */
    int unused[4];        /* 0x20 */
} material_effect_material; /* 48 bytes */
