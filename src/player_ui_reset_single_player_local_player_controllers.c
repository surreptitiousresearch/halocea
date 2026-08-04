#include "headers/player_ui_globals.h"

void player_ui_reset_single_player_local_player_controllers(void)
{
    player_ui_globals.single_player_controller_index[0] = -1;
    player_ui_globals.single_player_controller_index[1] = -1;
}
