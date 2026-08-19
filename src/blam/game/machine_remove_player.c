/* machine_remove_player @0x836A7AF0 — clear the slot holding player_index in the machine's entry of
 * machine_to_player_table (setting it back to -1). Each machine owns up to 2 player slots; if the player
 * is not found in the first 2 slots the call is a no-op. */

#include "headers/blam_data_globals.h"

void machine_remove_player(int machine_index, int player_index)
{
    /* (8*i) & 0x7FFF8 == 8*(i & 0xFFFF) — 2 int slots per 16-bit machine index */
    int *entry = &machine_to_player_table[2 * (machine_index & 0xFFFF)];
    int slot_count = 0;
    for ( int *slot = entry; *slot != player_index; ++slot )
    {
        if ( ++slot_count >= 2 )
            return;
    }

    entry[slot_count] = -1;
}
