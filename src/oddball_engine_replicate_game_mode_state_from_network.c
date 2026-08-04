/* oddball_engine_replicate_game_mode_state_from_network @0x83816DB0 — (client) apply a replicated
 * Oddball game mode state: incremental decodes seed the ENTIRE live oddball_globals from the baseline
 * first (decode_incremental only touches fields present in the delta), decode, then resync just
 * individual_score/team_score/current_ball_owner in the baseline from the now-updated live struct;
 * stateless decodes go straight into the baseline. On a successful decode, individual_score/team_score/
 * current_ball_owner are (re)synced from the baseline into the live struct, and — unless the ball type is
 * "terminator" — every team_score and individual_score entry is rescaled by 30 from what was just synced. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/oddball_globals.h"
#include "headers/game_variant.h"
#include "headers/blam_data_globals.h"


extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern unsigned __int8 message_delta_processor_decode_incremental(void *destination_data, void *baseline_data,
        const message_delta_processor_header *header, unsigned __int8 allow_empty_body);
extern void *memcpy(void *destination, const void *source, unsigned int size);
extern game_variant *game_engine_get_variant(void);

void oddball_engine_replicate_game_mode_state_from_network(message_delta_processor_header *header,
        network_game_client *client)
{
    unsigned __int8 decoded;

    if ( header->decoding_information->mode )
    {
        memcpy(&oddball_globals, &oddball_globals_baseline, sizeof(oddball_globals));
        decoded = message_delta_processor_decode_incremental(&oddball_globals, &oddball_globals_baseline, header, 0);

        memcpy(oddball_globals_baseline.individual_score, oddball_globals.individual_score,
            sizeof(oddball_globals_baseline.individual_score));
        memcpy(oddball_globals_baseline.team_score, oddball_globals.team_score,
            sizeof(oddball_globals_baseline.team_score));
        memcpy(oddball_globals_baseline.current_ball_owner, oddball_globals.current_ball_owner,
            sizeof(oddball_globals_baseline.current_ball_owner));
    }
    else
    {
        decoded = message_delta_processor_decode_stateless(&oddball_globals_baseline, header);
    }

    if ( decoded == 1 )
    {
        memcpy(oddball_globals.individual_score, oddball_globals_baseline.individual_score,
            sizeof(oddball_globals.individual_score));
        memcpy(oddball_globals.team_score, oddball_globals_baseline.team_score,
            sizeof(oddball_globals.team_score));
        memcpy(oddball_globals.current_ball_owner, oddball_globals_baseline.current_ball_owner,
            sizeof(oddball_globals.current_ball_owner));

        if ( game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type != _oddball_terminator )
        {
            for ( int i = 0; i < 16; i++ )
            {
                oddball_globals.team_score[i] *= 30;
                oddball_globals.individual_score[i] *= 30;
            }
        }
    }
}
