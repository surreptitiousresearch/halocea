/* default_starting_location_rate_function @0x8374DF08 — the default spawn-point rating: start from the
 * distance rating (forced to zero when the engine enforces team play and the spawn belongs to another team),
 * scale by the friendly-spawn bonus in team variants, then by the game-engine-specific rating hook. */

#include <stdint.h>
#include "headers/game_engine.h"
#include "headers/player_datum.h"
#include "headers/scenario_player.h"
#include "headers/game_variant.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern float game_engine_get_distance_rating_for_spawn(int player_index, const scenario_player *starting_location);
extern float game_engine_get_friendly_bonus(int player_index, const scenario_player *starting_location);

float default_starting_location_rate_function(int player_index, scenario_player *starting_location)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    struct game_engine *engine = game_engine; /* game_engine is a bare struct tag */

    float rating;
    uint8_t enforce_team = 0;
    if (engine && engine->game_engine_test_flag)
        enforce_team = engine->game_engine_test_flag(0);

    if (engine && enforce_team && player->team_index != starting_location->team_index)
        rating = 0.0f;
    else
        rating = game_engine_get_distance_rating_for_spawn(player_index, starting_location);

    engine = game_engine;
    if (!engine)
        return rating;

    if (rating > 0.0f && global_variant.universal_variant.teams)
        rating = game_engine_get_friendly_bonus(player_index, starting_location) * rating;

    engine = game_engine;
    if (engine && engine->starting_location_rating)
        rating = engine->starting_location_rating(player_index, starting_location) * rating;

    return rating;
}
