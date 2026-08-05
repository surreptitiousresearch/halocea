#pragma once
/* action_queue — a circular queue of buffered player actions plus the last-known-good action,
 * used to smooth/replay remote player input. Layout from the database (60 bytes). */

#include <stdint.h>
#include "simple_circular_queue.h"
#include "player_action.h"
#include "action_entry.h"

typedef struct action_queue
{
    simple_circular_queue queue;                 /* 0x00 */
    action_entry         *entry_bank;            /* 0x14 */
    uint8_t       has_last_valid_action; /* 0x18 */
    unsigned char _pad0[3]; /* db-verified padding */
    player_action         last_valid_action;     /* 0x1C */
} action_queue;                                  /* 60 bytes */
