/* reset_ball @0x838160F0 — (server only) return an oddball to a spawn position: look up a fresh position
 * for this ball (by the weapon's netgame-equipment slot word at weapon+184), play the ball-reset
 * multiplayer sound when the variant runs few enough balls for it to matter (ball_spawn_count <= 2), warp
 * the flag/ball object there, and clear the weapon's in-play flag (bit 6, 0x40, of the flags dword +556). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/game_variant.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/game_connection.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"
#include "headers/weapon_flags.h"


extern int16_t game_connection(void);
extern real_point3d * find_position_for_ball(real_point3d *result, int ball_index);
extern game_variant *game_engine_get_variant(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern void game_engine_flag_reset(int weapon_index, const real_point3d *position);

void reset_ball(int weapon_index)
{
    if ( game_connection() != _game_connection_network_server )
        return;

    weapon_datum *weapon =
        (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;

    real_point3d found_position[2];
    real_point3d position = *find_position_for_ball(found_position, weapon->object.owner_team_index);

    if ( game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count <= 2 )
        game_engine_play_multiplayer_sound(_multiplayer_sound_hill_move, 1u);

    game_engine_flag_reset(weapon_index, &position);
    weapon->weapon.flags &= ~(1u << _weapon_oddball_flag_has_been_touched_bit);
}
