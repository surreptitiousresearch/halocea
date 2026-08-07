/* client_team_change_from_network @0x83802F80 — (client) apply a replicated player/team-change message:
 * decode the (player_index, team_index) pair, and if the local machine is the client (game_connection==1),
 * find the matching network_player record by player_list_index and update its team byte directly before
 * also calling change_team_for_player_in_player_datum; if no record matches, only the player-datum update
 * runs.
 *
 * NOT a base this reconstruction dropped (re-derived 2026-08-07): `li r11, 0x142` @0x83802FF0 encodes as
 * 39600142, rA = r0. 0x142 + 32k IS network_game_data.players[k] (DB types_members) over the NULL that the
 * network_game_client_get_game() stub folds in — mechanism in network_game_server_get_game.c. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/network_player.h"
#include "headers/game_connection.h"

extern uint8_t game_engine_has_teams(void);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern int16_t game_connection(void);
extern network_game_client *global_network_game_client_get(void);
extern void change_team_for_player_in_player_datum(int player_index, uint8_t team_index);

void client_team_change_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode || !game_engine_has_teams() )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    struct
    {
        uint8_t player_index;
        uint8_t team_index;
    } payload = { (uint8_t)-1, (uint8_t)-1 };

    if ( !message_delta_processor_decode_stateless(&payload, header) || game_connection() != _game_connection_network_client )
        return;

    global_network_game_client_get();

    for ( network_player *record = (network_player *)0x142; (char *)record < (char *)0x542;
          ++record )   /* stride 32 == sizeof(network_player); typed element advance */
    {
        if ( record->player_list_index == payload.player_index )
        {
            record->team_index = payload.team_index;
            change_team_for_player_in_player_datum(payload.player_index, payload.team_index);
            return;
        }
    }
    change_team_for_player_in_player_datum(payload.player_index, payload.team_index);
}
