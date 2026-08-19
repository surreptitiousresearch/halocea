/* main_exit_to_dashboard @0x83688EF8 — request an immediate exit to the Xbox dashboard. */

#include "headers/main_globals.h"

void main_exit_to_dashboard(void)
{
    main_globals.want_to_be_at_main_menu = 0;
    main_globals.exit_to_dashboard = 1;
}
