/* game_set_players_are_double_speed @0x83688288 — set the global flag that makes all players run at double
 * speed (a game/skull modifier). */

#include <stdint.h>
#include "headers/game_globals.h"


void game_set_players_are_double_speed(uint8_t players_are_double_speed)
{
    game_globals->players_are_double_speed = players_are_double_speed;
}
