/* terminal_update_input @0x836A3170 — pumps keyboard input into the active console gets() session:
 * buffers each keystroke (up to 32), feeds it to the inline edit-text field, and blinks the insertion
 * caret on a 500ms timer. Returns 1 while a session is active, 0 otherwise. */

#include <stdint.h>
#include "headers/terminal_globals.h"
#include "headers/key_stroke.h"

#include "headers/edit_text.h"
extern uint32_t system_milliseconds(void);
extern uint8_t input_abstraction_get_key(key_stroke *key);
extern void edit_text_handle_key(edit_text *edit, const key_stroke *key);

int terminal_update_input(void)
{
    if (!terminal_globals.input_state)
        return 0;

    int now = system_milliseconds();
    key_stroke key[2];

    for (terminal_globals.input_state->key_count = 0;
         input_abstraction_get_key(key);
         terminal_globals.insertion_point_visible = 1)
    {
        terminal_gets_state *input_state = terminal_globals.input_state;
        int key_count = input_state->key_count;
        if (key_count < 32)
        {
            input_state->keys[key_count] = key[0];
            ++input_state->key_count;
        }
        edit_text_handle_key(&input_state->edit, key);
        terminal_globals.last_insertion_point_toggle = now;
    }

    if (now > terminal_globals.last_insertion_point_toggle + 500)
    {
        terminal_globals.last_insertion_point_toggle = now;
        /* DEVIATION: (cntlzw(x) & 0x20) != 0 is the "x == 0" idiom — i.e. toggle the caret. */
        terminal_globals.insertion_point_visible = !terminal_globals.insertion_point_visible;
    }
    return 1;
}
