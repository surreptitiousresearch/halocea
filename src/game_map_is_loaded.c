/* game_map_is_loaded @0x83687B50 */
#include "headers/game_globals.h"

int game_map_is_loaded(void)
{
    return game_globals->map_loaded;
}
