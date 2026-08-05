#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* saved_film_game_tick — the decoded per-tick record written to a saved film (function-local layout
 * reconstructed from save_film_record_one_game_tick's stack: 1056 bytes). Holds the update number, RNG
 * seed, game-state CRC, the player count actually carried, and the per-player action snapshot. The
 * `unused` slot is left uninitialized by the encoder path (matching the original). */

#include <stdint.h>
#include "player_action.h"

typedef struct saved_film_game_tick
{
    unsigned int  update_number;       /* 0x000 */
    unsigned int  random_seed;         /* 0x004 */
    unsigned int  unused;              /* 0x008 — never written */
    unsigned int  crc;                 /* 0x00C */
    int16_t       _pad;                /* 0x010 */
    int16_t       player_count;        /* 0x012 — 0 when actions are unchanged from the previous tick */
    player_action action_updates[32];  /* 0x014 */
    char          _tail[12];           /* 0x414 */
} saved_film_game_tick;                /* 1056 bytes */
