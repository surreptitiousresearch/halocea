#include <stdint.h>
#include "headers/player_ui_globals.h"

void player_ui_set_single_player_local_player_controller(int16_t local_player_index, int16_t controller_index)
{
    player_ui_globals.single_player_controller_index[local_player_index] = controller_index;
}
