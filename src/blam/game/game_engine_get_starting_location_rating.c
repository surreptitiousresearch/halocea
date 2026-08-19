/* game_engine_get_starting_location_rating @0x8374E038 — rate a candidate spawn point for a player: zero if
 * the location's game-type mask doesn't include the active game engine type or a vehicle is nearby, otherwise
 * defer to the default starting-location rating function. */

#include <stdint.h>
#include "headers/game_engine.h"
#include "headers/scenario_player.h"
#include "headers/real_point3d.h"

extern uint8_t match_game_type(int game_engine_type, int count, const int16_t *game_type);
extern uint8_t nearby_vehicle(const real_point3d *center);
extern float default_starting_location_rate_function(int player_index, scenario_player *starting_location);

float game_engine_get_starting_location_rating(int player_index, scenario_player *starting_location)
{
    unsigned int type = (unsigned int)-1;
    if (game_engine)
        type = game_engine->type;

    /* The nearby_vehicle call site loads only r3 (the starting_location); the second prototype argument is
     * not set up here, so the effective call passes starting_location as the leading argument. */
    if (!match_game_type(type, 4, starting_location->game_type)
        || nearby_vehicle((const real_point3d *)starting_location)) /* attested 1-param: phantom 2nd arg dropped */
        return 0.0f;

    return default_starting_location_rate_function(player_index, starting_location);
}
