/* game_statistics_start @0x83755180 — mark the game-statistics subsystem active. */

#include "headers/game_statistics_globals.h"

void game_statistics_start(void)
{
    game_statistics_globals.active = 1;
}
