/* terminal_gets_begin @ 0x836A30C0 — start an input session bound to a gets state, pointing its
 * edit buffer at the result field. Fails (returns 0) if another session is already active. */

#include <stdint.h>
#include "headers/terminal_globals.h"

extern void edit_text_new(edit_text *edit);

uint8_t terminal_gets_begin(terminal_gets_state *state)
{
    if ( terminal_globals.input_state )
        return 0;

    terminal_globals.input_state = state;
    state->edit.buffer = state->result;
    state->edit.maximum_length = 255;
    edit_text_new(&state->edit);
    state->key_count = 0;
    return 1;
}
