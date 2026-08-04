/* player_create_from_network @0x836A91D8 — (client) apply a "player create" message-delta: decode the
 * new player's network-player-list slot, global player index, translated index, and team, then spawn the
 * local player_datum slot (unless it already exists) and register the translated index, add it to client
 * bookkeeping and score tracking, seed the new datum's team/timer/action fields, and bind it to a local
 * player slot if it is one of ours.
 *
 * DEVIATION: the decompiler splits the decode destination into four separate locals (a `_BYTE[4]` array
 * followed by three bare ints); they are one contiguous by-ref decode buffer, reproduced here as a single
 * struct — same pattern as other *_from_network decode call sites in this codebase. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/network_game_data.h"
#include "headers/network_player.h"
#include "headers/field_properties_definition.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);

extern network_game_data *network_game_get_game(void);
extern void *datum_try_and_get(const data_array *data, int index);
extern uint8_t network_game_spawn_player_client(network_player *player, int player_index);
extern uint8_t field_translated_index_client_register_local_and_translated_index(const _field_properties_definition *const field_properties_definition, const int local_index, const int translated_index);
extern int update_client_add_player(int player_index);
extern void game_engine_player_score_add_player(int player_index);
extern void game_engine_player_added(int player_index);
extern void local_player_set_player_index(int16_t local_player_index, int player_index);

typedef struct _player_create_message
{
    unsigned char player_list_index;
    unsigned char _pad[3];
    int           player_index;
    int           translated_index;
    int           team_index;
} _player_create_message;

void player_create_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    _player_create_message message;
    if ( !message_delta_processor_decode_stateless(&message, header) )
        return;

    network_player *player = &network_game_get_game()->players[message.player_list_index];
    player_datum *player_datum_ptr = datum_try_and_get(player_data, message.player_index);

    if ( !player_datum_ptr )
    {
        if ( network_game_spawn_player_client(player, message.player_index) != 1 )
            return;

        field_translated_index_client_register_local_and_translated_index(&field_properties_player_index_definition,
            message.player_index, message.translated_index);
        player_datum_ptr = datum_try_and_get(player_data, message.player_index);
        update_client_add_player(message.player_index);
        game_engine_player_score_add_player(message.player_index);
        if ( !player_datum_ptr )
            return;
    }

    player_datum_ptr->team_index = message.team_index;
    player_datum_ptr->network_player_data.team_index = (char)message.team_index;

    if ( player_datum_ptr->local_player_index != -1 )
        local_player_set_player_index(player->controller_index, message.player_index);

    player_datum_ptr->powerup_durations[0] = 0;
    player_datum_ptr->powerup_durations[1] = 0;
    player_datum_ptr->action_result = 0;
    player_datum_ptr->action_object_index = -1;

    game_engine_player_added(message.player_index);
}
