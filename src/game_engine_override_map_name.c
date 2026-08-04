/* game_engine_override_map_name @0x83749668 — copies a non-empty map_name into global_stage.map_name. */

#include <string.h>

#include "headers/play_stage.h"
#include "headers/blam_data_globals.h"


void game_engine_override_map_name(char *map_name)
{
    if (map_name && *map_name)
        strncpy(global_stage.map_name, map_name, 0x3Fu);
}
