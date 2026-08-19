/* hud_activate_team_nav_point_with_flag @0x837ED238 — thunk activating a team nav point anchored to a CTF/
 * oddball flag (_nav_point_flag), forwarding to the general hud_activate_team_nav_point. */

#include <stdint.h>
#include "headers/hud_nav_point_type.h"

extern void hud_activate_team_nav_point(int16_t nav_index, int16_t team_index, int16_t type, int reference_index, float vertical_offset);

void hud_activate_team_nav_point_with_flag(int16_t nav_index, int16_t team_index, int16_t flag_index, float vertical_offset)
{
    hud_activate_team_nav_point(nav_index, team_index, _nav_point_flag, flag_index, vertical_offset);
}
