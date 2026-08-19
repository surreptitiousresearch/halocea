/* player_control_action_test_jump @0x836DF090 — return the latched "jump" action-test bit (bit 1 of the
 * player control globals' action_test_flags). */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/player_control_action_test_flags.h"


uint8_t player_control_action_test_jump(void)
{
    return (player_control_globals->action_test_flags >> _player_control_action_test_jump_bit) & 1;
}
