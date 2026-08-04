/* input_abstraction_initialize @ 0x83725648 — probe gamepads, reset input state */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/input_abstraction_mode_flags.h"

extern void *memset(void *, int, unsigned int);
extern uint8_t input_has_gamepad(int16_t gamepad_index);
extern uint32_t system_milliseconds(void);

/* return void: r3 at blr is loop residue (extsh r3,gamepad_index), not `now`; 0 consumers.
 * The decompiler threaded `now` into r3 as a phantom return — dropped. */
extern void input_abstraction_flush_configuration_button_pressed(void);
void input_abstraction_initialize(void)
{
    unsigned int now;
    int i;

    memset(&input_abstraction_globals, 0, sizeof(input_abstraction_globals));
    device_usage_info.last_used_index = 0;
    for ( i = 0; i < 4; ++i )
        input_abstraction_globals.controller_available[i] = input_has_gamepad(i);

    now = system_milliseconds();
    input_abstraction_globals.device_enumeration_startup_timer = now;
    /* DEVIATION: collapsed literal inlined copy of input_abstraction_flush_configuration_button_pressed@0x83724F18 (zero-xref donor; void/void, no args to fold) back into a call */
    input_abstraction_flush_configuration_button_pressed();
    input_abstraction_globals.initialized = 1;
    input_abstraction_globals.active_modes |= (1u << _input_abstraction_mode_game_bit);
}
