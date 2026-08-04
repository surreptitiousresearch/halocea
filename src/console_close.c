/* console_close @ 0x83721E70 — end an active console input session. */

#include <stdint.h>
#include "headers/console_globals.h"

extern void terminal_gets_end(terminal_gets_state *state);
extern void input_abstraction_set_enabled_mode_keyboard(uint8_t enabled);

void console_close(void)
{
    if ( console_globals.active && console_globals.available )
    {
        terminal_gets_end(&console_globals.input_state);
        console_globals.active = 0;
        input_abstraction_set_enabled_mode_keyboard(0);
    }
}
