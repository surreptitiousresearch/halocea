/* cheats_initialize @ 0x83699E38 — clear persistent cheat state */

#include "headers/cheat_globals.h"

void cheats_initialize(void)
{
    cheat.deathless_player = 0;
}
