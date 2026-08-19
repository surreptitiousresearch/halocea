/* console_update @ 0x83722B50 — per-frame in-game console (terminal) update. Toggles the
 * console open/closed on the aux ("~") button, and while open dispatches the captured
 * keystrokes: command-history recall, clipboard paste, tab-completion, and command submit
 * on enter. Returns the console's active flag.
 *
 * The decompiler reached the recalled previous command via raw pointer math off
 * &input_state.edit.selection_start_index; this is reproduced semantically using the
 * previous_commands ring buffer (same memory, clearer intent). */

#include <stdint.h>
#include "headers/console_globals.h"
#include "headers/key_code.h"
#include "headers/auxbutton.h"

extern uint8_t input_abstraction_auxbutton_down(int16_t button);
extern void console_open(void);
extern void console_close(void);
extern int console_process_command(const char *command, uint16_t extra_flags);
extern uint8_t console_clipboard_paste(void);
extern void console_complete(void);
extern void edit_text_selection_reset(edit_text *edit);
extern char *strcpy(char *dst, const char *src);

/* recall a previous command (offset back from the newest) into the input line */
static void console_recall_previous(int offset)
{
    int slot = (console_globals.newest_previous_command_index - offset + 8) % 8;
    strcpy(console_globals.input_state.result, console_globals.previous_commands[slot]);
    console_globals.input_state.edit.insertion_point_index = 0;
    console_globals.input_state.edit.selection_start_index = -1;
    edit_text_selection_reset(&console_globals.input_state.edit);
}

uint8_t console_update(void)
{
    int key_index;

    if ( !console_globals.available )
        return console_globals.active;

    if ( input_abstraction_auxbutton_down(_auxbutton_console) == 1 )
    {
        if ( console_globals.active )
            console_close();
        else
            console_open();
        return console_globals.active;
    }

    if ( !console_globals.active )
        return console_globals.active;

    for ( key_index = 0; key_index < console_globals.input_state.key_count; ++key_index )
    {
        switch ( console_globals.input_state.keys[key_index].key_code )
        {
            case _key_up_arrow: /* history up */
                if ( console_globals.selected_previous_command_index < console_globals.previous_command_count )
                    ++console_globals.selected_previous_command_index;
                console_recall_previous(console_globals.selected_previous_command_index);
                break;

            case _key_down_arrow: /* history down */
                if ( console_globals.selected_previous_command_index > 0 )
                    --console_globals.selected_previous_command_index;
                console_recall_previous(console_globals.selected_previous_command_index);
                break;

            case _key_f6: /* paste */
                console_clipboard_paste();
                break;

            case _key_tab: /* tab-complete */
                console_complete();
                break;

            case _key_return: /* enter */
            case _keypad_enter:
                if ( console_globals.input_state.result[0] )
                    console_process_command(console_globals.input_state.result, 0); /* disasm: r4=extra_flags=0 */
                else
                    console_close();
                break;
        }
    }

    return console_globals.active;
}
