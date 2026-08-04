/* player_ui_end_editing_profile @0x83699450 — end profile editing by invalidating the edited profile index. */

#include "headers/player_ui_globals.h"

void player_ui_end_editing_profile(void)
{
    player_ui_globals.edit_profile_data.profile_index = -1;
}
