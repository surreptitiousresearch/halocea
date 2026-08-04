/* hud_unit_deactivate_nav_point_with_flag @0x837EE144 — deactivate a flag-anchored HUD nav point for
 * the player controlling the given unit (no-op if the unit isn't player-controlled). */

#include <stdint.h>
#include "headers/hud_nav_point_type.h"

extern int player_index_from_unit_index(int unit_index);
extern void hud_deactivate_nav_point(int player_index, int16_t type, int reference_index);

void hud_unit_deactivate_nav_point_with_flag(int unit_index, int16_t flag_index)
{
    int player_index = player_index_from_unit_index(unit_index);
    if ( player_index != -1 )
        hud_deactivate_nav_point(player_index, _nav_point_flag, flag_index);
}
