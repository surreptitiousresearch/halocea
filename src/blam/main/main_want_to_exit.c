/* main_want_to_exit @0x83688F18 — request that the application quit, terminating the main loop. */

#include "headers/main_globals.h"
#include "headers/blam_data_globals.h"


void main_want_to_exit(void)
{
    main_globals.want_to_be_at_main_menu = 0;
    main_globals.want_to_exit = 1;
    globalQuit = 1;
}
