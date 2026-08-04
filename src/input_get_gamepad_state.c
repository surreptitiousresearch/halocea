/* input_get_gamepad_state @ 0x836FBC58 — pointer to a controller's gamepad state (40-byte
 * stride into input_globals.gamepads). */

#include "headers/input_globals.h"

#include <stdint.h>

gamepad_state *input_get_gamepad_state(int16_t gamepad_index)
{
    /* recovered: (char *)&input_globals + 40 * gamepad_index -> gamepads[gamepad_index] */
    return &input_globals.gamepads[gamepad_index];
}
