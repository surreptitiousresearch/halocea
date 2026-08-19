/* terminal_gets_end @ 0x836A3128 — end an input session (only if it owns the terminal). */

#include "headers/terminal_globals.h"

void terminal_gets_end(terminal_gets_state *state)
{
    if ( terminal_globals.input_state == state )
        terminal_globals.input_state = 0;
}
