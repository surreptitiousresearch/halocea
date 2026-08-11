/* main_get_map_name @0x83688FC0 — return the currently selected single-player map name. */

#include "headers/main_globals.h"

char *main_get_map_name(void)
{
    return main_globals.soloplayer_map_name;
}
