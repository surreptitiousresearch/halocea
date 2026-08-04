#pragma once
/* player_action_collection — the saved actions for the local players on one machine (split screen). */

#include "player_action.h"

typedef struct player_action_collection
{
    player_action actions[2];   /* 0x00 — one per local player */
} player_action_collection;     /* 64 bytes */
