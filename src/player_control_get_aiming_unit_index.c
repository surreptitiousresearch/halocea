#include <stdint.h>
#include "headers/player_control_globals.h"

extern int unit_get_aiming_unit_index(int unit_index);

int player_control_get_aiming_unit_index(int16_t local_player_index)
{
    return unit_get_aiming_unit_index(player_control_globals->players[local_player_index].unit_index);
}
