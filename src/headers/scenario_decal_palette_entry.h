#pragma once
/* scenario_decal_palette_entry — one entry of the scenario's decal palette block
 * (16 bytes, DB types_members-confirmed). */

#include "tag_reference.h"

typedef struct scenario_decal_palette_entry
{
    tag_reference reference; /* 0x00 — 'deca' tag (.index @ 0x0C) */
} scenario_decal_palette_entry; /* 16 bytes */
