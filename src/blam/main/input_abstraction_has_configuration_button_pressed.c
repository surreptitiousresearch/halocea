/* input_abstraction_has_configuration_button_pressed @0x83724E90 — true if input is currently in
 * "configuration" active-mode (bit 0x8, taking priority over the other active-mode bits) and a
 * configuration button press has actually been captured (device_type != 0). */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/input_abstraction_mode_flags.h"

extern int input_abstraction_get_current_mode(void);
uint8_t input_abstraction_has_configuration_button_pressed(void)
{
    /* DEVIATION: verbatim inlined copy of input_abstraction_get_current_mode@0x83724BD8 (zero-xref donor) collapsed to a direct call; donor takes no params so no arg folding is needed. */
    int mode = input_abstraction_get_current_mode();

    return mode == _input_abstraction_mode_configuration_bit
        && input_abstraction_globals.configuration_button_pressed.device_type != 0;
}
