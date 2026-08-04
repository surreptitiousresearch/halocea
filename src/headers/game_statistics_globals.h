#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* game_statistics_globals — game-statistics recording state (game_statistics.c). Only the active flag
 * (offset 0) is reached by reconstructed callers. */

typedef struct game_statistics_globals_t
{
    unsigned char active; /* 0x00 */
} game_statistics_globals_t;

extern game_statistics_globals_t game_statistics_globals;
