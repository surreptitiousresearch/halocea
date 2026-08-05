/* race_update_team_score @0x8382A268 — recompute every team's race score (race_globals.team_laps[]) from
 * its players' individual lap-progress values (player_datum+396, unresolved server/client update-data
 * union field), combined per the CTF-shared single_flag_time setting: 0 = take the first qualifying
 * player's value, 1 = take the maximum across the team, else = sum across the team. When
 * single_flag_time==2, the team's persistent_team_score (the array immediately following team_laps in
 * race_globals_type — read here via the same "adjacent array" pointer-walk idiom the compiler used) is
 * added in. After recomputing all 16 teams, replicates the updated score/state to the network and ends the
 * game for any team that has reached the variant's score_to_win.
 *
 * DEVIATION: reproduced with the original's goto/label control flow intact (rather than restructured)
 * since the single_flag_time-dependent branches interleave in a way that's easy to get subtly wrong by
 * "cleaning up" — faithfulness over readability here. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/race_globals.h"
#include "headers/game_variant.h"
#include "headers/message_delta_processor_mode.h"
#include "headers/blam_data_globals.h"


extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern game_variant *game_engine_get_variant(void);
extern void game_engine_replicate_score_and_state_to_network(message_delta_processor_mode mode, int machine_index);
extern void game_engine_end_game(void);

game_variant *race_update_team_score(void)
{
    int team_index = 0;
    int *team_laps = race_globals.team_laps;

    do
    {
        data_iterator iterator;
        data_iterator_new(&iterator, player_data);

        int accumulated = 0;
        int count = 0;
        int16_t single_flag_time = 0;
        int player_value = 0;

        player_datum *player = data_iterator_next(&iterator);
        if ( player )
        {
            while ( 1 )
            {
                if ( player->team_index != team_index || player->quit_out_of_game )
                    goto next_player;

                /* +396 = race lap-progress, overlaying the server_update_data union arm. */
                player_value = player->___u26.server_update_data.time_of_last_remote_player_vehicle_update;
                single_flag_time = game_engine_get_variant()->game_engine_variant.ctf.single_flag_time;

                if ( !single_flag_time )
                    goto first_qualifying_only;

                if ( single_flag_time == 1 )
                {
                    if ( count && accumulated > player_value )
                        goto increment_count;
                    goto take_player_value;
                }

                accumulated += player_value;

            increment_count:
                ++count;

            next_player:
                player = data_iterator_next(&iterator);
                if ( !player )
                    goto done_with_team;
            }

        first_qualifying_only:
            if ( count && accumulated <= player_value )
                goto increment_count;
        take_player_value:
            accumulated = player_value;
            goto increment_count;
        }

    done_with_team:
        team_laps[0] = accumulated;
        if ( game_engine_get_variant()->game_engine_variant.ctf.single_flag_time == 2 )
            team_laps[0] += team_laps[16];

        ++team_laps;
        ++team_index;
    }
    while ( (int)team_laps < (int)race_globals.persistent_team_score );

    game_engine_replicate_score_and_state_to_network(_message_delta_mode_incremental, -1);

    game_variant *result = nullptr;
    int *team_score = race_globals.team_laps;
    do
    {
        result = game_engine_get_variant();
        if ( *team_score >= result->universal_variant.score_to_win )
            game_engine_end_game(); /* game_engine_end_game attested void — was cast-of-void residue */
        ++team_score;
    }
    while ( (int)team_score < (int)race_globals.persistent_team_score );

    return result;
}
