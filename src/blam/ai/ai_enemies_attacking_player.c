/* ai_enemies_attacking_player @0x836E98B8 — true if any AI enemy is actively attacking a player.
 * Thin wrapper: forwards to ai_enemies_endanger_player with must_be_attacking = 1. */

#include <stdint.h>

extern uint8_t ai_enemies_endanger_player(uint8_t must_be_attacking);

uint8_t ai_enemies_attacking_player(void)
{
    return ai_enemies_endanger_player(1);
}
