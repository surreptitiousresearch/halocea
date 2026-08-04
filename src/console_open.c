/* console_open @ 0x83721E00 — begin a console input session (unless already active,
 * unavailable, or the virtual keyboard is up). */

#include <stdint.h>
#include "headers/console_globals.h"

#include "headers/terminal_gets_state.h"
extern uint8_t virtual_keyboard_active(void);
extern uint8_t terminal_gets_begin(terminal_gets_state *state);
extern void input_abstraction_set_enabled_mode_keyboard(uint8_t enabled);

void console_open(void)
{
    if ( !console_globals.active && console_globals.available && !virtual_keyboard_active() )
    {
        console_globals.input_state.result[0] = 0;
        console_globals.active = terminal_gets_begin(&console_globals.input_state);
        input_abstraction_set_enabled_mode_keyboard(1u);
    }
}
