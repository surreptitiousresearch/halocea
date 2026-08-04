#pragma once
/* player_update_history — a per-player singly-linked queue of pending networked updates, plus playback
 * statistics. Layout from the database. */

#include "history_playback_statistics.h"

struct player_update;

typedef struct player_update_history
{
    int                   next_update_id;        /* 0x00 */
    struct player_update *update_history_head;   /* 0x04 */
    struct player_update *update_history_tail;   /* 0x08 */
    history_playback_statistics playback_stats;  /* 0x0C */
} player_update_history;                         /* 36 bytes */
