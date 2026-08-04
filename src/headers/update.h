#pragma once
/* update — one numbered network update: a sequence number and the game_update payload. */

#include "game_update.h"

typedef struct update
{
    int         update_number;   /* 0x000 */
    game_update data;            /* 0x004 */
} update;                        /* 1160 bytes */
