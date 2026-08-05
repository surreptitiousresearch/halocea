/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* oddball_engine_player_killed_player @0x838178C0 — oddball/terminator kill bookkeeping (server, special ball
 * types only). On a non-suicide non-friendly kill it updates the killer's ball-kill stats, decides whether the
 * kill scores (mode-dependent: game_engine_can_score for terminator kills of a carrier / by a carrier,
 * ball_available otherwise) and awards a point, then — if the killer has a live unit — transfers the ball to the
 * killer: it locates the victim's ball slot (or the first free, non-spawning slot), shows the "you/ally/enemy"
 * score message, and assigns that slot to the killer. In all cases it finally clears the dead player from every
 * ball-owner slot. No-op unless the variant is a special ball type (_oddball_normal < type <= _oddball_terminator)
 * and this is the server.
 *
 * The killer's player datum (stride 512) is read at raw offsets: kill-a-carrier stat @+198, kill-while-carrying
 * stat @+200, unit_index @+52. oddball_globals.current_ball_owner[16] (the decompiler's `*(v15-16)` is
 * ball_spawn_timer[i], 16 ints below current_ball_owner). The goto-based dispatch is reflowed into structured
 * control flow. */

#include <stdint.h>
#include "headers/oddball_globals.h"
#include "headers/game_variant.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/oddball_message.h"
#include "headers/blam_data_globals.h"


extern game_variant *game_engine_get_variant(void);
extern int16_t game_connection(void);
extern uint8_t player_has_ball(int player_index);
extern uint8_t ball_available(void);
extern uint8_t game_engine_can_score(void);
extern void oddball_add_score(int player_index);
extern void game_show_score_you_ally_enemy(int player_index, int message_you, int message_ally, int message_enemy, int message_data, uint8_t should_replicate);

void oddball_engine_player_killed_player(int killing_player_index, int killing_object_index, int dead_player_index, uint8_t friendly_fire)
{
    oddball_ball_type ball_type = game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type;
    bool special_ball = ball_type > _oddball_normal && ball_type <= _oddball_terminator;
    if ( !special_ball || game_connection() != _game_connection_network_server )
        return;

    int ball_spawn_count = game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count;

    if ( killing_player_index != -1 && !friendly_fire )
    {
        player_datum *killer = DATA_ARRAY_ELEMENT(player_data, player_datum, killing_player_index);
        uint8_t can_score = 0;
        bool run_scoring = true;

        if ( player_has_ball(dead_player_index) )
        {
            ++killer->statistics.multiplayer_statistics.oddball_statistics.ball_carrier_kills;
            if ( ball_type == _oddball_terminator )
                can_score = game_engine_can_score();
            else
                run_scoring = false;
        }
        else if ( !player_has_ball(killing_player_index) )
        {
            can_score = ball_available();
        }
        else
        {
            ++killer->statistics.multiplayer_statistics.oddball_statistics.kills_as_ball_carrier;
            if ( ball_type == _oddball_terminator )
                can_score = game_engine_can_score();
            else
                run_scoring = false;
        }

        if ( run_scoring && can_score )
            oddball_add_score(killing_player_index);

        if ( killer->unit_index != -1 )
        {
            if ( ball_spawn_count <= 0 )
                return;

            int slot_index = -1;
            for ( int i = 0; ; )
            {
                if ( oddball_globals.ball_spawn_timer[i] == 0 && slot_index == -1
                  && oddball_globals.current_ball_owner[i] == -1 )
                    slot_index = i;
                if ( oddball_globals.current_ball_owner[i] == dead_player_index )
                {
                    slot_index = i;
                    break;
                }
                if ( ++i >= ball_spawn_count )
                    break;
            }

            if ( slot_index != -1 )
            {
                oddball_ball_type bt = game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type;
                bool special = bt > _oddball_normal && bt <= _oddball_terminator;
                game_show_score_you_ally_enemy(killing_player_index,
                        special ? -1 : oddball_message_you_are_it,
                        oddball_message_ally_is_it, oddball_message_enemy_is_it,
                        killing_player_index, 0);
                oddball_globals.current_ball_owner[slot_index] = killing_player_index;
            }
        }
    }

    if ( ball_spawn_count > 0 )
    {
        for ( int i = 0; i < ball_spawn_count; ++i )
            if ( oddball_globals.current_ball_owner[i] == dead_player_index )
                oddball_globals.current_ball_owner[i] = -1;
    }
}
