#pragma once
/* material_effect — one effect-type record in a 'foot' material-effects tag: a block of
 * per-material responses (28 bytes, DB types_members-confirmed). */

#include "tag_block.h"

typedef struct material_effect
{
    tag_block materials; /* 0x00 — material_effect_material[] */
    int unused[4];       /* 0x0C */
} material_effect;       /* 28 bytes */
