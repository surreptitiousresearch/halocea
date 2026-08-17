/* machine_remove_all_players @0x836A7B38 */
#include "headers/blam_data_globals.h"

void machine_remove_all_players(int machine_index)
{
    /* (8*i) & 0x7FFF8 == 8*(i & 0xFFFF) — 2 int slots per 16-bit machine index */
    int *entry = &machine_to_player_table[2 * (machine_index & 0xFFFF)];
    entry[0] = -1;
    entry[1] = -1;
}
