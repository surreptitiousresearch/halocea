/* hud_deactivate_team_nav_point_with_flag @0x837ED5F0 — thunk deactivating a flag-anchored
 * (_nav_point_flag) team nav point, forwarding to hud_deactivate_team_nav_point. */

#include <stdint.h>
#include "headers/hud_nav_point_type.h"

extern void hud_deactivate_team_nav_point(int16_t team_index, int16_t type, int reference_index);

void hud_deactivate_team_nav_point_with_flag(int16_t team_index, int16_t flag_index)
{
    hud_deactivate_team_nav_point(team_index, _nav_point_flag, flag_index);
}
