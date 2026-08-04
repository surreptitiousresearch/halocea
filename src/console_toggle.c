#include "headers/console_globals.h"

extern void console_close(void);
extern void console_open(void);

void console_toggle(void)
{
    if (console_globals.active)
        console_close(); /* attested void */
    else
        console_open(); /* attested void */
}
