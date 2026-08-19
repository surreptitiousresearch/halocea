/* king_engine_player_update @0x8382C0B8 — per-frame King-of-the-Hill update for one player. Clears the player's
 * on-the-hill flag, then (only while scoring is allowed and the player has a slave unit) tests whether that unit
 * is standing on the hill: the unit origin's z must lie within [hill_bottom, hill_top] and its xy must fall
 * inside the hill's convex hull. If so, the player is marked on-hill; on the host (connection == 2) the player's
 * on-hill tick counter is bumped and, once per game tick, the owning team's score is incremented. Reaching 900 /
 * 1800 ticks below the win threshold fires the "30 seconds" / "60 seconds" announcer sounds (team-specific when
 * teams are enabled), every 150 ticks plays a personal scoring cue, and hitting the threshold ends the game.
 *
 * DEVIATION: Hex-Rays rendered game_engine_end_game as taking `variant`; disasm (0x8382C314-0x8382C31C) shows no
 * argument is set up before the call (r3 was clobbered by the preceding call), so it is invoked with no
 * arguments — variant is only used locally for the 1800*score_to_win threshold. The score-tick counter, hill
 * bounds, hull, and per-team score/score_tick arrays come from king_globals. */

#include <stdint.h>
#include "headers/king_globals.h"
#include "headers/game_variant.h"
#include "headers/real_point2d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/object_datum.h"
#include "headers/game_connection.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/king_message.h"
#include "headers/blam_data_globals.h"


extern void game_engine_state_message(int player_index, int message, int message_data);
extern uint8_t game_engine_can_score(void);
extern uint8_t convex_hull2d_test_point(int16_t count, const real_point2d *points, const real_point2d *point, float epsilon);
extern int16_t game_connection(void);
extern int game_time_get(void);
extern game_variant *game_engine_get_variant(void);
extern uint8_t game_engine_has_teams(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern void game_engine_play_multiplayer_sound_to_specific_player(int player_index, int index, uint8_t should_replicate);
extern void game_engine_end_game(void);


void king_engine_player_update(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    uint16_t player_slot = (uint16_t)player_index;

    game_engine_state_message(player_index, -1, -1);
    king_globals.on_the_hill[player_slot] = 0;

    if ( player->unit_index == -1 || !game_engine_can_score() )
        return;

    uint8_t on_hill = 0;
    if ( player_index != -1 )
    {
        int unit_index = player->unit_index;
        if ( unit_index != -1 )
        {
            object_datum *unit = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
            float z = unit->object.bounding_sphere_center.n[2];
            if ( z >= king_globals.hill_bottom && z <= king_globals.hill_top )
            {
                real_point2d position;
                position.n[0] = unit->object.bounding_sphere_center.n[0];
                position.n[1] = unit->object.bounding_sphere_center.n[1];
                on_hill = convex_hull2d_test_point(king_globals.hill_point_count,
                        king_globals.convex_hull, &position, 0.0f);
            }
        }
    }

    if ( !on_hill )
        return;

    king_globals.on_the_hill[player_slot] = 1;
    if ( game_connection() == _game_connection_network_server )
        ++player->statistics.multiplayer_statistics.king_statistics.time_on_hill;

    int team = player->team_index;
    if ( king_globals.score_tick[team] < game_time_get() && game_connection() == _game_connection_network_server )
    {
        ++king_globals.score[team];
        king_globals.score_tick[team] = game_time_get();

        game_variant *variant = game_engine_get_variant();
        int score_to_win_ticks = 1800 * variant->universal_variant.score_to_win;
        int score = king_globals.score[team];

        if ( score_to_win_ticks - score == 900 )
        {
            int sound = game_engine_has_teams()
                ? (team == 0 ? _multiplayer_sound_red_30_seconds : _multiplayer_sound_blue_30_seconds)
                : _multiplayer_sound_30_seconds;
            game_engine_play_multiplayer_sound(sound, 1);
        }
        if ( score_to_win_ticks - score == 1800 )
        {
            int sound = game_engine_has_teams()
                ? (team == 0 ? _multiplayer_sound_red_60_seconds : _multiplayer_sound_blue_60_seconds)
                : _multiplayer_sound_60_seconds;
            game_engine_play_multiplayer_sound(sound, 1);
        }

        int current_score = king_globals.score[team];
        if ( current_score > 0 && !(current_score % 150) && current_score < score_to_win_ticks )
            game_engine_play_multiplayer_sound_to_specific_player(player_index, _multiplayer_sound_countdown_timer_end, 1);
        if ( score >= score_to_win_ticks )
            game_engine_end_game();
    }

    game_engine_state_message(player_index, king_message_you_are_on_the_hill, player_index);
}
