/* player_control_action_test_zoom @0x836DF0D8 — return the latched "zoom" action-test bit (bit 6 of the player control globals'
 * action_test_flags). */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/player_control_action_test_flags.h"

uint8_t player_control_action_test_zoom(void)
{
    return (player_control_globals->action_test_flags >> _player_control_action_test_zoom_bit) & 1;
}
