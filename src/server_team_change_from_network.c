/* server_team_change_from_network @0x83802D58 — (server) handle a client's replicated team-change request:
 * decode the (player_index, requested_team) pair; for an explicit team (0 or 1) reject it when autobalance is
 * on and the team is full (informing the player), and for any other value pick a balanced team via
 * find_new_team_for_player_to_join. If a valid team results and it differs from the player's current team,
 * commit it to the server and client network_player records, to the player datum, and to the game engine, then
 * re-broadcast the authoritative team change to every loaded machine. Only runs while a team game engine is
 * active; otherwise the iteration body is discarded.
 *
 * Faithful shipped quirk (bug class 12, same as client_team_change_from_network / teamplay_get_team_info): the
 * two record-search loops walk the 32 network_player slots at the ABSOLUTE address 0x142 (base 0x142 + 32*i),
 * not through the fetched server/client pointer — the global_network_game_*_get() results are discarded
 * (disasm 0x83802DF0/0x83802E00). Reproduced verbatim. The player-datum scan matches on network_player_data.player_list_index (byte +103). */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/game_variant.h"
#include "headers/network_player.h"
#include "headers/network_game_server.h"
#include "headers/network_game_client.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/multiplayer_team.h"
#include "headers/blam_data_globals.h"

extern int network_message_type_message_delta;
extern int _message_team_change;

extern uint8_t game_engine_running(void);
extern uint8_t game_engine_has_teams(void);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern game_variant *game_engine_get_variant(void);
extern uint8_t can_player_join_team(int player_index, unsigned int team);
extern void server_info_message_to_player(int player_index, int strings_dll_message_id);
extern uint8_t find_new_team_for_player_to_join(int player_index);
extern network_game_server *global_network_game_server_get(void);
extern network_game_client *global_network_game_client_get(void);
extern void change_team_for_player_in_player_datum(int player_index, uint8_t team_index);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern void game_engine_player_changed_team(int player_index, int new_team);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void server_team_change_from_network(message_delta_processor_header *const header)
{
    if ( header->decoding_information->mode != _message_delta_mode_stateless
      || !game_engine_running() || !game_engine_has_teams() )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    struct
    {
        uint8_t player_index;
        uint8_t team_index;
    } payload = { (uint8_t)-1, (uint8_t)-1 };

    if ( !message_delta_processor_decode_stateless(&payload, header) )
        return;

    game_variant *variant = game_engine_get_variant();
    uint8_t new_team = payload.team_index;
    if ( payload.team_index == _multiplayer_team_red || payload.team_index == _multiplayer_team_blue )
    {
        if ( variant->universal_variant.team_autobalance
          && !can_player_join_team(payload.player_index, payload.team_index) )
        {
            server_info_message_to_player(payload.player_index, 145);
            return;
        }
    }
    else
    {
        new_team = find_new_team_for_player_to_join(payload.player_index);
    }

    if ( new_team == (uint8_t)-1 )   /* disasm 0x83802DE8 */
        return;

    global_network_game_server_get();        /* discarded; loop walks the fixed 0x142 table */

    uint8_t team_changed = 0;
    for ( network_player *record = (network_player *)0x142; (char *)record < (char *)0x542;
          ++record )   /* stride 32 == sizeof(network_player); typed element advance */
    {
        if ( record->player_list_index == payload.player_index )
        {
            uint8_t old_team = record->team_index;
            record->team_index = new_team;
            team_changed = new_team != old_team;
            break;
        }
    }

    if ( team_changed )
    {
        global_network_game_server_get();
        global_network_game_client_get();
        for ( network_player *record = (network_player *)0x142; (char *)record < (char *)0x542;
              ++record )   /* stride 32 == sizeof(network_player); typed element advance */
        {
            if ( record->player_list_index == payload.player_index )
            {
                record->team_index = new_team;
                break;
            }
        }
        change_team_for_player_in_player_datum(payload.player_index, new_team);

        data_iterator iterator;
        data_iterator_new(&iterator, player_data);
        for ( player_datum *player = data_iterator_next(&iterator); player;
              player = data_iterator_next(&iterator) )
        {
            /* byte @+103 = network_player_data.player_list_index (char) — same signed-char read */
            if ( player->network_player_data.player_list_index == payload.player_index )
            {
                game_engine_player_changed_team(iterator.index, new_team);
                break;
            }
        }

        int message_size = message_delta_processor_encode_stateless(_message_team_change, nullptr, &payload,
                g_message_encode_buffer, 32760);
        network_game_server *server = global_network_game_server_get();
        network_game_server_send_message_to_all_loaded_machines(server, network_message_type_message_delta,
                g_message_encode_buffer, message_size, 1, 0, 1, 3);
    }
}
