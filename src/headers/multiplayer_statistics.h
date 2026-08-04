#pragma once
/* multiplayer_statistics — the game-mode-specific score block carried in a player's score update (6 bytes).
 * DB union of per-mode views (slayer/ctf/oddball/king/race); member names verbatim from the DB (each arm
 * is named after its type). Consumers that touch the raw 16-bit slots go through the mode-specific arm. */

#include "slayer_statistics.h"
#include "ctf_statistics.h"
#include "oddball_statistics.h"
#include "king_statistics.h"
#include "race_statistics.h"

typedef union multiplayer_statistics
{
    slayer_statistics  slayer_statistics;  /* 0x00 */
    ctf_statistics     ctf_statistics;     /* 0x00 */
    oddball_statistics oddball_statistics; /* 0x00 */
    king_statistics    king_statistics;    /* 0x00 */
    race_statistics    race_statistics;    /* 0x00 */
} multiplayer_statistics; /* 6 bytes */
