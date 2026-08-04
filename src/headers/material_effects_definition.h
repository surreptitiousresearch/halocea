#pragma once
/* material_effects_definition — a 'foot' tag: block of material_effect records
 * (140 bytes, DB types_members-confirmed). */

#include "tag_block.h"

typedef struct material_effects_definition
{
    tag_block effects; /* 0x00 — material_effect[] */
    int unused[32];    /* 0x0C */
} material_effects_definition; /* 140 bytes */
