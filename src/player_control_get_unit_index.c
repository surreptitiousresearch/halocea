/* player_control_get_unit_index @0x836DE950 — return the object index of the unit a local player currently
 * controls. */

#include <stdint.h>
#include "headers/player_control_globals.h"

int player_control_get_unit_index(int16_t local_player_index)
{
    return player_control_globals->players[local_player_index].unit_index;
}
