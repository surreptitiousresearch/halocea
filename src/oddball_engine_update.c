/* oddball_engine_update @0x83817738 — per-tick Oddball update. At the 60th tick, plays the "get ball"
 * announcement (variant with/without teams). On the server, ticks down each ball's spawn timer and calls
 * create_the_ball() when one reaches 0. For juggernaut-style ball types (magic/terminator), refreshes each
 * ball's multiplayer goal marker every tick: clears it if the ball is unowned, otherwise re-targets it at
 * the current owner's object position (offset 0xA0) as long as the owner still has a valid player index.
 *
 * DEVIATION: disasm_range(0x83817820,0x838178A0) resolves the same `game_engine_set_goal_position` ABI-
 * shadow-register bug already confirmed at 2 other call sites (race_engine_initialize_for_new_map.c,
 * king_engine_update.c) — the float `vertical_offset` (0.0f here) consumes a shadow GPR slot, shifting the
 * decompiler's shown args one register early. Real mapping at this site: descriptor="target_blue" (r6),
 * player_index=-1 (r7), team_index=-1 (r8). The trailing `ignore_player_index` (r9) is NOT set by this call
 * at all — disasm traces r9 back to `subfe r9, r10, r10` inside the unrelated tick==60 announcement branch
 * (a leftover boolean-sign-extension byproduct, only even reachable on some calls), so it is genuine
 * leftover register garbage, not a real argument. Reproduced as a 0 placeholder, matching the established
 * precedent in king_engine_update.c for find_hill's similarly-garbage trailing args. */

#include <stdint.h>
#include "headers/oddball_globals.h"
#include "headers/player_datum.h"
#include "headers/game_variant.h"
#include "headers/oddball_ball_type.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/game_connection.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern uint8_t game_engine_has_teams(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern game_variant *game_engine_get_variant(void);
extern int16_t game_connection(void);
extern void create_the_ball(int ball_index);
extern void game_engine_clear_goal_position(int16_t index);
extern void game_engine_set_goal_position(int16_t index, real_point3d *position, float vertical_offset, char *descriptor, int player_index, int16_t team_index, int ignore_player_index);

void oddball_engine_update(void)
{
    if ( game_time_get() == 60 )
    {
        unsigned __int8 has_teams = game_engine_has_teams();
        game_engine_play_multiplayer_sound(
            has_teams == 0 ? _multiplayer_sound_oddball : _multiplayer_sound_team_oddball, 0);
    }

    int ball_spawn_count = game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count;

    if ( game_connection() == _game_connection_network_server )
    {
        for ( int i = 0; i < ball_spawn_count; ++i )
        {
            if ( oddball_globals.ball_spawn_timer[i] > 0 )
            {
                if ( --oddball_globals.ball_spawn_timer[i] == 0 )
                {
                    game_engine_play_multiplayer_sound(_multiplayer_sound_oddball_spawn, 0);
                    create_the_ball(i);
                }
            }
        }
    }

    oddball_ball_type oddball_ball_type = game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type;
    unsigned __int8 has_ball_markers = oddball_ball_type > _oddball_normal && oddball_ball_type <= _oddball_terminator;

    if ( has_ball_markers )
    {
        for ( int i = 0; i < ball_spawn_count; ++i )
        {
            int owner_object_index = oddball_globals.current_ball_owner[i];
            if ( owner_object_index == -1 )
            {
                game_engine_clear_goal_position(i);
            }
            else
            {
                int owner_player_index = DATA_ARRAY_ELEMENT(player_data, player_datum, owner_object_index)->unit_index;
                if ( owner_player_index != -1 )
                {
                    real_point3d *owner_position = (real_point3d *)
                        (((int)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, owner_object_index)->datum) + 0xA0);
                    game_engine_set_goal_position(i, owner_position, 0.0f, "target_blue", -1, -1, 0);
                }
            }
        }
    }
}
