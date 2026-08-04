/* ai_enemies_can_see_player @0x836E98B0 — true if any AI enemy currently has line of sight to a player.
 * Thin wrapper: forwards to ai_enemies_endanger_player with must_be_attacking = 0 (sight alone counts). */

#include <stdint.h>

extern uint8_t ai_enemies_endanger_player(uint8_t must_be_attacking);

uint8_t ai_enemies_can_see_player(void)
{
    return ai_enemies_endanger_player(0);
}
