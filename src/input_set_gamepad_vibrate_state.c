#include <stdint.h>
/* input_set_gamepad_vibrate_state @0x836FB5C8 — set a gamepad's rumble motor speeds. Compiled to a bare
 * `blr` (empty no-op) in this build; reproduced faithfully from the disassembly. */

void input_set_gamepad_vibrate_state(int16_t gamepad_index, uint16_t left_speed, uint16_t right_speed)
{
    (void)gamepad_index;
    (void)left_speed;
    (void)right_speed;
}
