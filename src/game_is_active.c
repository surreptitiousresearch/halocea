/* game_is_active @0x83687B60 */
#include "headers/game_globals.h"

int game_is_active(void)
{
    return game_globals->active != 0;
}
