#pragma once
#include <stdint.h>
/* game_data_input_reference — one element of a ui_widget_definition's game_data_input_references
 * tag block (36 bytes per DB types_members). function is the game-data function selector at +0. */

typedef struct game_data_input_reference
{
    int16_t function;       /* 0x00 */
    int16_t unused_flags;   /* 0x02 */
    int     unused[8];      /* 0x04 */
} game_data_input_reference; /* 36 bytes */
