/* hud_deactivate_nav_point_with_game_engine_flag @0x837ED498 — tail-call forwarder (`extsh r5, r4;
 * li r4, 2; b hud_deactivate_nav_point`; confirmed via disasm, not a bare blr). Forwards with type=2
 * (game-engine-flag nav point) and flag_index sign-extended into reference_index. */

#include <stdint.h>
#include "headers/hud_nav_point_type.h"

extern void hud_deactivate_nav_point(int player_index, int16_t type, int reference_index);

void hud_deactivate_nav_point_with_game_engine_flag(int player_unit_index, int16_t flag_index)
{
    hud_deactivate_nav_point(player_unit_index, _nav_point_game_engine_flag, flag_index);
}
