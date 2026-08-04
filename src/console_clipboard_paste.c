/* console_clipboard_paste @ 0x83721ED0 — paste clipboard text into the console input. */

#include <stdint.h>
#include "headers/console_globals.h"

#include "headers/terminal_gets_state.h"
extern uint8_t shell_get_clipboard_text(char *buffer, int length);
extern void terminal_gets_insert_string(terminal_gets_state *state, const char *string);

uint8_t console_clipboard_paste(void)
{
    char clipboard[256];
    uint8_t result;

    result = shell_get_clipboard_text(clipboard, 255);
    if ( result )
        terminal_gets_insert_string(&console_globals.input_state, clipboard);
    return result;
}
