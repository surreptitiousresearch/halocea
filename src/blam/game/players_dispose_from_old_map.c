/* players_dispose_from_old_map @ 0x836A7A08 — invalidate player + team pools */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern void data_make_invalid(data_array *data);
void players_dispose_from_old_map(void)
{
    data_make_invalid(player_data);
    data_make_invalid(team_data);
}
