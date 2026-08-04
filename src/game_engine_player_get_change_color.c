/* game_engine_player_get_change_color 0x83748CF0 — fill `result` with the change/armor color used to tint a
 * player's model. In a team game this is the team color (blue for team index != 0, else red); otherwise it is the
 * player's chosen profile color. */

#include "headers/real_rgb_color.h"
#include "headers/game_variant.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


extern real_rgb_color *player_profile_get_rgb_color(real_rgb_color *result, int index);

real_rgb_color *game_engine_player_get_change_color(real_rgb_color *result, int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if ( global_variant.universal_variant.teams )
    {
        /* non-zero = blue team, zero = red team */
        const real_rgb_color *team_color = player->team_index ? global_real_rgb_blue : global_real_rgb_red;
        result->n[0] = team_color->n[0];
        result->n[1] = team_color->n[1];
        result->n[2] = team_color->n[2];
        return result;
    }

    /* no teams: the player's chosen profile color index */
    real_rgb_color profile_color;
    const real_rgb_color *rgb = player_profile_get_rgb_color(&profile_color, player->network_player_data.primary_color_index);
    result->n[0] = rgb->n[0];
    result->n[1] = rgb->n[1];
    result->n[2] = rgb->n[2];
    return result;
}
