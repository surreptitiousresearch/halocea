/* game_players_are_double_speed @0x83688298 — true when the game is running at double simulation
 * speed (e.g. the speed cheat / certain script states). */

#include <stdint.h>
#include "headers/game_globals.h"

uint8_t game_players_are_double_speed(void)
{
    return game_globals->players_are_double_speed;
}
