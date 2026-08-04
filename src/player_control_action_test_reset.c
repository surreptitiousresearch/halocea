/* player_control_action_test_reset @0x836DEFE8 — clear the action-test latch and the "testing for action"
 * mask, starting a fresh action-test frame. */

#include "headers/player_control_globals.h"

void player_control_action_test_reset(void)
{
    player_control_globals->action_test_flags = 0;
    player_control_globals->testing_for_action_flags = 0;
}
