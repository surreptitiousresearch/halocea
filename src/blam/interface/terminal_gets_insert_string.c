/* terminal_gets_insert_string @ 0x836A3150 — insert text at the caret of the active session. */

#include "headers/terminal_globals.h"

extern void edit_text_insert_string(edit_text *edit, const char *string);

void terminal_gets_insert_string(terminal_gets_state *state, const char *string)
{
    if ( state == terminal_globals.input_state )
        edit_text_insert_string(&terminal_globals.input_state->edit, string);
}
