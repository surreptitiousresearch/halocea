/* hud_activate_team_nav_point_with_game_engine_flag @0x837ED228 — tail-call forwarder (`extsh r6, r5;
 * li r5, 2; b hud_activate_team_nav_point`; confirmed via disasm, not a bare blr). Forwards with type=2
 * (game-engine-flag nav point) and flag_index sign-extended into reference_index. */

#include <stdint.h>
#include "headers/hud_nav_point_type.h"

extern void hud_activate_team_nav_point(int16_t nav_index, int16_t team_index, int16_t type, int reference_index, float vertical_offset);

void hud_activate_team_nav_point_with_game_engine_flag(int16_t nav_index, int16_t team_index, int16_t flag_index, float vertical_offset)
{
    hud_activate_team_nav_point(nav_index, team_index, _nav_point_game_engine_flag, flag_index, vertical_offset);
}
