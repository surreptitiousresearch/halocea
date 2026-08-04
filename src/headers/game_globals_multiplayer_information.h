#pragma once
/* game_globals_multiplayer_information — element of game_globals.multiplayer_information
   (160 bytes; DB-verified). sounds elements are 16-byte tag_references. */

#include "tag_reference.h"
#include "tag_block.h"

typedef struct game_globals_multiplayer_information
{
    tag_reference flag;        /* 0x00 */
    tag_reference player_unit; /* 0x10 */
    tag_block     vehicles;    /* 0x20 */
    tag_reference hill_shader; /* 0x2C */
    tag_reference flag_shader; /* 0x3C */
    tag_reference ball;        /* 0x4C */
    tag_block     sounds;      /* 0x5C */
    int           unused[14];  /* 0x68 */
} game_globals_multiplayer_information; /* 160 bytes */
