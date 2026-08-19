/* player_control_action_test_grenade_trigger @0x836DF0C0 — return the latched "grenade trigger" action-test bit (bit 5 of the player control globals'
 * action_test_flags). */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/player_control_action_test_flags.h"

uint8_t player_control_action_test_grenade_trigger(void)
{
    return (player_control_globals->action_test_flags >> _player_control_action_test_grenade_trigger_bit) & 1;
}
