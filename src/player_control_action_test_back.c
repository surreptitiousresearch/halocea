/* player_control_action_test_back @0x836DF030 — test the latched "back/B" action (bit 3 of action_test_flags) and, as a side
 * effect, mark that action as being tested (testing_for_action_flags) and inhibit its raw button
 * (inhibit_button_flags) so it is not also consumed as a unit control this frame. */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/player_control_action_test_flags.h"

uint8_t player_control_action_test_back(void)
{
    unsigned int action_test_flags = player_control_globals->action_test_flags;
    player_control_globals->inhibit_button_flags |= (1u << _player_control_action_test_back_bit);
    player_control_globals->testing_for_action_flags |= (1u << _player_control_action_test_back_bit);
    return (action_test_flags >> _player_control_action_test_back_bit) & 1;
}
