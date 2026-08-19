/* ctf_engine_replicate_game_mode_state_from_network @0x838070D0 — (client) apply a replicated CTF game
 * mode state: stateless decodes go into the baseline; incremental decodes go into the live ctf_globals
 * (against the baseline), and — in single-flag CTF — a change of team_with_flag clears all four goal HUD
 * positions before the baseline is resynchronized from the live struct. On a successful decode the live
 * score pair and team_with_flag are (re)stored from the values just synchronized (the compiled code moves
 * score[0]/score[1] as one 8-byte copy; written as explicit element copies here). */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/ctf_globals.h"
#include "headers/game_variant.h"
#include "headers/blam_data_globals.h"


extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_incremental(void *const destination_data, const void *const baseline_data, const message_delta_processor_header *const header, const uint8_t allow_empty_body);
extern game_variant *game_engine_get_variant(void);
extern void game_engine_clear_goal_position(int16_t index);

void ctf_engine_replicate_game_mode_state_from_network(message_delta_processor_header *header,
        network_game_client *client)
{
    uint8_t decoded;
    int score0;
    int score1;
    uint8_t team_with_flag;

    if ( header->decoding_information->mode )
    {
        decoded = message_delta_processor_decode_incremental(&ctf_globals, &ctf_globals_baseline, header, 0);

        if ( game_engine_get_variant()->game_engine_variant.ctf.single_flag_time > 0
          && ctf_globals.team_with_flag != ctf_globals_baseline.team_with_flag )
        {
            game_engine_clear_goal_position(0);
            game_engine_clear_goal_position(1);
            game_engine_clear_goal_position(2);
            game_engine_clear_goal_position(3);
        }

        score0 = ctf_globals.score[0];
        score1 = ctf_globals.score[1];
        team_with_flag = ctf_globals.team_with_flag;
        ctf_globals_baseline.score[0] = score0;
        ctf_globals_baseline.score[1] = score1;
        ctf_globals_baseline.team_with_flag = team_with_flag;
    }
    else
    {
        decoded = message_delta_processor_decode_stateless(&ctf_globals_baseline, header);
        score0 = ctf_globals_baseline.score[0];
        score1 = ctf_globals_baseline.score[1];
        team_with_flag = ctf_globals_baseline.team_with_flag;
    }

    if ( decoded == 1 )
    {
        ctf_globals.score[0] = score0;
        ctf_globals.score[1] = score1;
        ctf_globals.team_with_flag = team_with_flag;
    }
}
