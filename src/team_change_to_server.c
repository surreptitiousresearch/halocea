/* team_change_to_server @0x83802960 — request a team change from a client: find the first local player's
 * current team (player datum +103), build a "team change" message body {current_team, new_team_choice},
 * encode it as a stateless message-delta, and (when it produced bits) hand it to the network client.
 * DEVIATION: the disassembly issues a single bl to global_network_game_client_get with the encode result
 * in r3 and discards it (the message send appears identical-code-folded into that symbol); reproduced. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_engine_running(void);
extern uint8_t game_engine_has_teams(void);
extern int _message_team_change;
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_client *global_network_game_client_get(void);

void team_change_to_server(uint8_t new_team_choice)
{
    if ( game_engine_running() && game_engine_has_teams() )
    {
        char current_team = -1;
        data_iterator iterator;
        data_iterator_new(&iterator, player_data);
        player_datum *player = data_iterator_next(&iterator);
        if ( player )
        {
            while ( (unsigned __int16)player->local_player_index == 0xFFFF )   /* skip remote players */
            {
                player = data_iterator_next(&iterator);
                if ( !player )
                    goto encode;
            }
            current_team = player->network_player_data.player_list_index;
        }
encode:
        {
            unsigned char message_body[16];
            message_body[1] = new_team_choice;
            message_body[0] = current_team;
            if ( message_delta_processor_encode_stateless(_message_team_change, 0, message_body, g_message_encode_buffer, 32760) > 0 )
                global_network_game_client_get();
        }
    }
}
