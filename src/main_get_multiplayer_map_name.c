/* main_get_multiplayer_map_name @0x83688FDC — return the currently selected multiplayer map name. */

#include "headers/main_globals.h"

char *main_get_multiplayer_map_name(void)
{
    return main_globals.multiplayer_map_name;
}
