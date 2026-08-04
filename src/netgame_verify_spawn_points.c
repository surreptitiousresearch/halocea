/* netgame_verify_spawn_points @0x8374E7F4 — walk every scenario player starting location (used to validate
 * netgame spawn points). The declared parameters are unused by the body — the function always iterates the
 * full starting-location list. */

#include <stdint.h>

#include "headers/scenario_player.h"
extern int16_t player_get_starting_location_count(void);
extern scenario_player * player_get_starting_location(int16_t location_index); /* result unused here */

void netgame_verify_spawn_points(int16_t game_engine, int16_t team, int16_t count, char *string)
{
    int starting_location_count = player_get_starting_location_count();
    for ( int16_t i = 0; i < starting_location_count; ++i )
        player_get_starting_location(i);
}
