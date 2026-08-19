/* terminal_gets_active @0x836A30A8 */
#include "headers/terminal_globals.h"

int terminal_gets_active(void)
{
    return terminal_globals.input_state != 0;
}
