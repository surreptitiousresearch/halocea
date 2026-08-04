/* ctf_engine_starting_location_rating @0x83806EA0 — rate how good a scenario_player starting location
 * is for a CTF-assault player, based on 3D distance to that team's flag (ctf_globals.flags[(team+1)%2] —
 * i.e. the OTHER team's flag). Not applicable to non-assault CTF variants (rating 1.0). Distance is
 * clamped to [0.5, 10.0]; the base rating is 1/distance. Early in the round (game_time <= 30 ticks) that
 * raw 1/distance rating is returned directly, with no further clamping. Afterward it's reshaped via
 * distance^0.33 (a gentler falloff curve) when the clamped distance exceeds 1.0, then clamped to
 * [0.5, 2.0]. */

#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/ctf_globals.h"
#include "headers/game_variant.h"
#include "headers/scenario_player.h"
#include "headers/blam_data_globals.h"


extern game_variant *game_engine_get_variant(void);
extern int game_time_get(void);
extern double pow(double base, double exponent);

float ctf_engine_starting_location_rating(int player_index, scenario_player *starting_location)
{
    if ( !game_engine_get_variant()->game_engine_variant.ctf.assault )
        return 1.0f;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    const scenario_netgame_flag *flag = ctf_globals.flags[(player->team_index + 1) % 2];

    float dx = flag->position.n[0] - starting_location->position.n[0];
    float dy = flag->position.n[1] - starting_location->position.n[1];
    float dz = flag->position.n[2] - starting_location->position.n[2];
    float distance_squared = dx * dx + dy * dy + dz * dz;

    if ( distance_squared < 0.5f )
        distance_squared = 0.5f;
    else if ( distance_squared > 10.0f )
        distance_squared = 10.0f;

    float rating = 1.0f / distance_squared;

    if ( game_time_get() <= 30 )
        return rating;

    if ( distance_squared > 1.0f )
        rating = (float)pow(1.0f / distance_squared, 0.33f);

    if ( rating < 0.5f )
        return 0.5f;
    if ( rating > 2.0f )
        return 2.0f;

    return rating;
}
