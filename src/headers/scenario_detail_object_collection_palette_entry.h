#pragma once
/* scenario_detail_object_collection_palette_entry — one element of scenario.detail_object_collection_palette
 * (48 bytes; DB-verified via types_members). A tag reference to a detail_object_collection plus padding. */

#include "tag_reference.h"

typedef struct scenario_detail_object_collection_palette_entry
{
    tag_reference reference;    /* 0x00 */
    int           unused[8];    /* 0x10 */
} scenario_detail_object_collection_palette_entry;   /* 48 bytes */
