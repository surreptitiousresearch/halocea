#include "headers/terminal_globals.h"

int terminal_gets_active(void)
{
    return terminal_globals.input_state != 0;
}
