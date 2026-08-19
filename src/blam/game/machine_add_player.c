/* machine_add_player @0x836A7AA8 — record player_index in the first free slot of the machine's entry in
 * machine_to_player_table. Each machine owns up to 2 player slots (terminated by -1); if both are already
 * occupied the call is a no-op. */

#include "headers/blam_data_globals.h"

void machine_add_player(int machine_index, int player_index)
{
    /* (8*i) & 0x7FFF8 == 8*(i & 0xFFFF) — 2 int slots per 16-bit machine index */
    int *entry = &machine_to_player_table[2 * (machine_index & 0xFFFF)];
    int slot_count = 0;
    for ( int *slot = entry; *slot != -1; ++slot )
    {
        if ( ++slot_count >= 2 )
            return;
    }

    entry[slot_count] = player_index;
}
