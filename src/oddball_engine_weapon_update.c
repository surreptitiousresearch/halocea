/* oddball_engine_weapon_update @0x83817510 — per-tick server update for an oddball "ball" weapon. Publishes the
 * ball's world position as the mode goal marker (so teammates get a nav point to it, ignoring the current
 * carrier). If the ball has been on the ground untouched for >0x4B0 ticks, on the server, and is a loose flag
 * object, it is reset: scored (message 37) if flagged, a pickup sound played when few balls are configured, and
 * respawned at a fresh spawn position. Finally, for normal/out-of-range ball types, once the per-team baseline
 * interval elapses it flags the ball object for a network baseline resend and stamps the baseline time.
 *
 * weapon_datum fields (now typed): object.flags @16, object.owner_team_index @184, object.parent_object_index @284,
 * item.last_owned_time @516, weapon.flags @556 — reached via weapon->object/item/weapon members.
 * DEVIATION: game_engine_set_goal_position takes a float vertical_offset that burns a GPR shadow slot, so the
 * decompiler mislabels the trailing args (see game_engine_set_goal_position.c) — the real call passes
 * descriptor "ball_blue", player_index -1, team_index -1, and ignore_player_index = current_ball_owner[team]
 * (the `lwzx r9` computed just before the call, disasm 0x8381756C). The ball-type membership test
 * (`_oddball_normal < type <= _oddball_terminator`) is the decompiler's short-circuit flag, cleaned up. */

#include <stdint.h>
#include "headers/weapon_datum.h"
#include "headers/_item_datum.h"
#include "headers/oddball_globals.h"
#include "headers/game_variant.h"
#include "headers/real_point3d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/game_connection.h"
#include "headers/object_flags.h"
#include "headers/weapon_flags.h"
#include "headers/oddball_message.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"


extern int item_get_position_even_if_in_inventory(int item_index, real_point3d *position);
extern void game_engine_set_goal_position(int16_t index, real_point3d *position, float vertical_offset, char *descriptor, int player_index, int16_t team_index, int ignore_player_index);
extern int game_time_get(void);
extern int16_t game_connection(void);
extern uint8_t weapon_is_flag(int weapon_index);
extern void game_show_score(int recipient_player_index, int subject_player_index, int message, uint8_t should_replicate);
extern real_point3d * find_position_for_ball(real_point3d *result, int ball_index);
extern game_variant *game_engine_get_variant(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern void game_engine_flag_reset(int weapon_index, const real_point3d *position);


void oddball_engine_weapon_update(int weapon_index, weapon_datum *weapon)
{
    real_point3d position;
    if ( item_get_position_even_if_in_inventory(weapon_index, &position) != 1 )
        return;

    /* recovered: *(__int16 *)(weapon_bytes + 184) -> weapon->object.owner_team_index */
    __int16 owner_team_index = weapon->object.owner_team_index;
    game_engine_set_goal_position(owner_team_index, &position, 0.0f, "ball_blue", -1, -1,
                                  oddball_globals.current_ball_owner[owner_team_index]);

    /* recovered: raw offsets +516/+16/+284/+556 -> weapon->{item,object,weapon} members */
    if ( (unsigned int)(game_time_get() - weapon->item.last_owned_time) > 0x4B0
      && game_connection() == _game_connection_network_server
      && weapon_is_flag(weapon_index)
      && ((weapon->object.flags >> _object_connected_to_map_bit) & 1) != 0
      && weapon->object.parent_object_index == -1 )
    {
        if ( (weapon->weapon.flags & (1u << _weapon_oddball_flag_has_been_touched_bit)) != 0 )
            game_show_score(-1, -1, oddball_message_ball_was_reset, 1);
        if ( game_connection() == _game_connection_network_server )
        {
            real_point3d ball_scratch[5];
            real_point3d *ball_position = find_position_for_ball(ball_scratch, weapon->object.owner_team_index);
            real_point3d spawn_position;
            spawn_position.n[0] = ball_position->n[0];
            spawn_position.n[1] = ball_position->n[1];
            spawn_position.n[2] = ball_position->n[2];
            if ( game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count <= 2 )
                game_engine_play_multiplayer_sound(_multiplayer_sound_hill_move, 1);
            game_engine_flag_reset(weapon_index, &spawn_position);
            weapon->weapon.flags &= ~(1u << _weapon_oddball_flag_has_been_touched_bit);
        }
    }

    if ( game_connection() == _game_connection_network_server )
    {
        oddball_ball_type ball_type = game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type;
        bool special_ball = ball_type > _oddball_normal && ball_type <= _oddball_terminator;
        if ( !special_ball )
        {
            /* recovered: *(__int16 *)(weapon_bytes + 184) -> weapon->object.owner_team_index */
            int team_index = weapon->object.owner_team_index;
            unsigned char baseline_due = 0;
            if ( oddball_globals.last_oddball_baseline_time[team_index] == -1
              || game_time_get() > oddball_globals.last_oddball_baseline_time[team_index] + oddball_baseline_rate )
                baseline_due = 1;
            if ( baseline_due == 1 )
            {
                /* recovered: raw offsets +284/+16 -> weapon->object.{parent_object_index,flags} */
                if ( weapon->object.parent_object_index == -1 )
                {
                    unsigned int flags = weapon->object.flags;
                    if ( (flags & (1u << _object_at_rest_bit)) != 0 )
                        weapon->object.flags = flags | (1u << _object_force_baseline_update_bit);
                }
                oddball_globals.last_oddball_baseline_time[team_index] = game_time_get();
            }
        }
    }
}
