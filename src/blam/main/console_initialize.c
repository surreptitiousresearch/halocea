/* console_initialize @ 0x83721D70 — reset the in-game console state */

#include "headers/console_globals.h"

extern char *strcpy(char *, const char *);

/* return attested void: r3 at exit is only the first word of console_color left over from the
 * color copy (lwz r3 @0x83721D90), and the sole caller (main_loop_init1) ignores it. */
void console_initialize(void)
{
    console_globals.input_state.color = console_color;
    strcpy(console_globals.input_state.prompt, "halo( ");
    console_globals.input_state.result[0] = 0;
    console_globals.newest_previous_command_index = -1;
    console_globals.previous_command_count = 0;
    console_globals.selected_previous_command_index = -1;
    console_globals.available = 1;
}
