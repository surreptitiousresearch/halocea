/* input_abstraction_set_enabled_mode_game @0x83724D08 */
#include <stdint.h>
#include "headers/input_abstraction_globals.h"

void input_abstraction_set_enabled_mode_game(uint8_t enabled)
{
    input_abstraction_globals.active_modes |= 1u;
}
