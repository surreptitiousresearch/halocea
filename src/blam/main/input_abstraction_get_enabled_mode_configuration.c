/* input_abstraction_get_enabled_mode_configuration @0x83724C80 — tests bit 0x8 of
 * input_abstraction_globals.active_modes. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/input_abstraction_mode_flags.h"

uint8_t input_abstraction_get_enabled_mode_configuration(void)
{
    return (input_abstraction_globals.active_modes >> _input_abstraction_mode_configuration_bit) & 1;
}
