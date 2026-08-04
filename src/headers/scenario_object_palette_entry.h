#pragma once
/* scenario_object_palette_entry — one element of a scenario object-type palette tag block
 * (48 bytes). Layout from the database (types_members). */

#include "tag_reference.h"

typedef struct scenario_object_palette_entry
{
    tag_reference reference;  /* 0x00 */
    unsigned int  unused[8];  /* 0x10 */
} scenario_object_palette_entry;  /* 48 bytes */
