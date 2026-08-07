/* vibrate_dispose_from_old_map @ 0x83739118-adjacent (0x83739528) — stop controller
 * rumble and zero the vibrate globals. */
#include <stdint.h>
#include <string.h>
#include "headers/vibrate_global_data.h"
extern void input_set_gamepad_vibrater_state(int, int, int);
extern uint8_t input_has_gamepad(int16_t gamepad_index);
void vibrate_dispose_from_old_map(void)
{
    int i;
    for ( i = 0; i < 2; ++i )
        input_set_gamepad_vibrater_state(i, 0, 0);
    memset(vibrate_globals, 0, sizeof(vibrate_global_data));
    for ( i = 0; i < 2; ++i )
    {
        if ( input_has_gamepad(i) )
            input_set_gamepad_vibrater_state(i, 0, 0);
    }
}
