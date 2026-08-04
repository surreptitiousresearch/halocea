/* map_list_free_known_list @ 0x83767618 — free the known multiplayer map list */

#include "headers/multiplayer_map_s.h"
#include "headers/blam_data_globals.h"

extern void dlFree(void *ptr);

void map_list_free_known_list(void)
{
    int i;

    for ( i = 0; i < multiplayer_map_list_count; ++i )
        dlFree(multiplayer_maps[i].name);

    dlFree(multiplayer_maps);
    multiplayer_maps = 0;
    multiplayer_map_list_count = 0;
    multiplayer_map_list_size = 0;
}
