#include "headers/blam_data_globals.h"
/* machine_get_player_list @0x836A7A90 — return a pointer to the per-machine player-list entry (8-byte stride)
 * in the global machine-to-player table. The `& 0x7FFF8` masks the byte offset to a 16-bit machine index. */


int *machine_get_player_list(int machine_index)
{
    /* (8*i) & 0x7FFF8 == 8*(i & 0xFFFF) — 2 int slots per 16-bit machine index */
    return &machine_to_player_table[2 * (machine_index & 0xFFFF)];
}
