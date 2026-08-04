#include "headers/blam_data_globals.h"
/* map_list_get_count @0x837672C0 — getter for the multiplayer map list count (see map_list_find_map.c for
 * the same global). */


int map_list_get_count(void)
{
    return multiplayer_map_list_count;
}
