/* map_list_get_level_name @0x837671D0 — getter for a multiplayer map list entry's name (see
 * map_list_find_map.c for the same global). */

#include "headers/multiplayer_map_s.h"
#include "headers/blam_data_globals.h"


char * map_list_get_level_name(int index)
{
    return multiplayer_maps[index].name;
}
