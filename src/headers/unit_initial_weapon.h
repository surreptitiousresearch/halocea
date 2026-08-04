#pragma once
/* unit_initial_weapon — one element of a unit definition's initial-weapons tag_block: a weapon tag
 * reference followed by unused padding. Layout DB-verified (types_members). */

#include "tag_reference.h"

typedef struct unit_initial_weapon
{
    tag_reference reference;   /* 0x00 (weapon tag; resolved index in reference.index) */
    int           unused[5];   /* 0x10 */
} unit_initial_weapon;         /* 36 bytes */
