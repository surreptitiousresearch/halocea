/* send_super_remote_players_action_update @0x83756DE0 — flush the accumulated batch of remote-player
 * action updates to the network. For each queued update whose target player datum still exists, collect
 * pointers to its header, action body, and the player's action delta baseline, and translate the player
 * index in place. The surviving set is then encoded as one iterated message-delta (stateless when the
 * batch is a baseline, otherwise incremental against each player's baseline) and broadcast to every
 * loaded machine. The batch counter is reset to empty afterward.
 *
 * The per-player action baseline lives at player_datum+320 (= server_update_data.remote_player_action_
 * update_baseline @ union+0x48). */

#include <stdint.h>
#include "headers/super_remote_players_action_update.h"
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/field_properties_definition.h"
#include "headers/network_game_server.h"
#include "headers/blam_data_globals.h"
#include "headers/message_delta_message_ids.h"
#include "headers/network_message_type.h"

typedef struct _field_properties_definition _field_properties_definition;
extern void *datum_try_and_get(const data_array *data, int index);
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);

extern int message_delta_processor_encode_stateless_iterated(message_delta_processor_message_definition_type definition_type, const void *const *headers, const void *const *datas, int iterations, void *buffer, int buffer_size_in_bits);
extern int message_delta_processor_encode_incremental_iterated(message_delta_processor_message_definition_type definition_type, const void *const *headers, const void *const *datas, const void *const *baselines, int iterations, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);

extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void send_super_remote_players_action_update(super_remote_players_action_update *update)
{
    const void *selected_headers[32];
    const void *selected_actions[32];
    const void *selected_baselines[32];
    int selected_count = 0;

    for (int i = 0; i < 32; i++)
    {
        selected_headers[i] = nullptr;
        selected_actions[i] = nullptr;
    }

    for (int i = 0; i < update->number_of_updates; i++)
    {
        player_datum *player = datum_try_and_get(player_data, update->network_data.headers[i].player_index);
        if (player)
        {
            selected_headers[selected_count] = &update->network_data.headers[i];
            selected_actions[selected_count] = &update->network_data.actions[i];
            selected_baselines[selected_count] = &player->___u26.server_update_data.remote_player_action_update_baseline;
            ++selected_count;
        }
    }

    for (int i = 0; i < selected_count; i++)
    {
        remote_player_action_update_header *header = (remote_player_action_update_header *)selected_headers[i];
        header->player_index =
            field_translated_index_translate_index(&field_properties_player_index_definition, header->player_index);
    }

    int size_in_bits;
    if (update->is_stateless == 1)
    {
        size_in_bits = message_delta_processor_encode_stateless_iterated(
            _message_super_remote_players_action_update, selected_headers, selected_actions,
            selected_count, g_message_encode_buffer, 32760);
    }
    else
    {
        size_in_bits = message_delta_processor_encode_incremental_iterated(
            _message_super_remote_players_action_update, selected_headers, selected_actions,
            selected_baselines, selected_count, g_message_encode_buffer, 32760, 1u);
    }

    network_game_server *server = global_network_game_server_get();
    network_game_server_send_message_to_all_loaded_machines(
        server, network_message_type_message_delta, g_message_encode_buffer, size_in_bits,
        update->is_stateless, 0, 0, 2);

    update->number_of_updates = 0;
}
