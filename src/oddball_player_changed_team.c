#include <stdint.h>
#include "headers/data_array.h"
#include "headers/oddball_globals.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);

void oddball_player_changed_team(int player_index, uint8_t new_team)
{
    if (datum_try_and_get(player_data, player_index))
        oddball_globals.individual_score[player_index] = 0;
}
