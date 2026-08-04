#pragma once
/* game_globals_tag_reference — a single tag_block element wrapping one tag_reference. Used by the game
 * globals tag's weapon_list / cheat_powerups blocks. */

#include "tag_reference.h"

typedef struct game_globals_tag_reference
{
    tag_reference reference;   /* 0x0 */
} game_globals_tag_reference;  /* 16 bytes */
