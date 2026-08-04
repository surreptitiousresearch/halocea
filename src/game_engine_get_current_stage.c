#include <string.h>

#include "headers/play_stage.h"

int game_engine_get_current_stage(game_variant *variant, char *map_name)
{
    memcpy(variant, &global_stage.game_variant, sizeof(game_variant));
    strncpy(map_name, global_stage.map_name, 0x3Fu);
    map_name[63] = 0;
    return 1;
}
