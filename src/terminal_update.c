/* terminal_update @ 0x836A3838 — pump terminal input, and output when console hidden */
#include <stdint.h>
#include "headers/terminal_globals.h"
extern int terminal_update_input(void);
extern void terminal_update_output(void);
extern uint8_t console_is_active(void);

int terminal_update(void)
{
    int updated;
    if ( !terminal_globals.initialized )
        return 0;
    updated = terminal_update_input();
    if ( !console_is_active() )
        terminal_update_output();
    return updated;
}
