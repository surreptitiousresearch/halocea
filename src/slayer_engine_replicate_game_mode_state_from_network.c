/* slayer_engine_replicate_game_mode_state_from_network @0x838159B0 — (client) apply a replicated Slayer
 * game mode state: stateless decodes go straight into the baseline; incremental decodes go into the live
 * slayer_globals (against the baseline). On a successful decode (result == 1) the baseline is resynced
 * from the live struct, and — when debug_score logging is enabled — every player's name is dumped to
 * ASCII (and, at the highest debug level, the team-mode check is re-run, though its result is discarded). */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/slayer_globals.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_incremental(void *destination_data, void *baseline_data,
        const message_delta_processor_header *header, uint8_t allow_empty_body);
extern char * wide_to_ascii(uint16_t *unicode, char *ascii, unsigned int ascii_length_bytes);
extern uint8_t game_engine_has_teams(void);
extern void *memcpy(void *destination, const void *source, unsigned int size);

void slayer_engine_replicate_game_mode_state_from_network(message_delta_processor_header *header,
        network_game_client *client)
{
    uint8_t decoded;

    if ( header->decoding_information->mode )
        decoded = message_delta_processor_decode_incremental(&slayer_globals, &slayer_globals_baseline, header, 0);
    else
        decoded = message_delta_processor_decode_stateless(&slayer_globals_baseline, header);

    if ( decoded == 1 )
    {
        memcpy(&slayer_globals, &slayer_globals_baseline, sizeof(slayer_globals));

        if ( debug_score > 0 )
        {
            data_iterator iterator;
            data_iterator_new(&iterator, player_data);
            for ( player_datum *player = data_iterator_next(&iterator); player;
                  player = data_iterator_next(&iterator) )
            {
                char ascii_name[1024];
                wide_to_ascii(player->name, ascii_name, 0x400u);
                if ( debug_score >= 1 )
                    game_engine_has_teams();
            }
        }
    }
}
