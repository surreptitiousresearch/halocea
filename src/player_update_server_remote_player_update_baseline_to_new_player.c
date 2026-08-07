/* player_update_server_remote_player_update_baseline_to_new_player @0x83756B68 — on the server, when
 * `machine_index` (a newly-joined machine) needs to catch up on an already-connected player
 * (`existing_player_index`), resend that player's three most recent baselines (action, position, vehicle)
 * to it as reliable stateless updates. Each baseline is copied straight out of the player's
 * player_update_server_data (the same baseline blocks build_remote_player_*_update.c stash there), so no
 * new state is computed — this just replays what the server already has on file.
 *
 * The vehicle message reuses the position update's header type, matching the documented sharing in
 * build_remote_player_vehicle_update.c ("The routing header is shared with the position update").
 *
 * DEVIATION: the raw decompile declares a fresh (seemingly uninitialized) local for the trailing `priority`
 * argument at each of the three send call sites (v15/v16/v17); disasm confirms all three read the same
 * stack slot loaded from a register set once to the literal 1 at function entry and never changed, so this
 * is genuinely the constant 1 three times over, not garbage — written as such directly.
 *
 * Also DEVIATION: the position/vehicle baseline blobs are copied dword-by-dword in the raw decompile
 * (Hex-Rays couldn't see them as the named structs); reproduced here as plain struct-sized memcpy()s from
 * the now-resolved headers/player_update_server_data.h baseline fields. */

#include <stdint.h>
#include <string.h>
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/field_properties_definition.h"
#include "headers/remote_player_action_update_header.h"
#include "headers/remote_player_action_update_network_data.h"
#include "headers/remote_player_position_update_header.h"
#include "headers/remote_player_position_update_network_data.h"
#include "headers/remote_player_vehicle_update_network_data.h"
#include "headers/network_game_server.h"
#include "headers/blam_data_globals.h"
#include "headers/message_delta_message_ids.h"
#include "headers/network_message_type.h"


typedef struct _field_properties_definition _field_properties_definition;


extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void player_update_server_remote_player_update_baseline_to_new_player(int existing_player_index, int machine_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, existing_player_index);

    remote_player_action_update_header action_header;
    action_header.player_index = field_translated_index_translate_index(&field_properties_player_index_definition,
            existing_player_index);
    action_header.update_id = player->___u26.server_update_data.update_id_for_action_baseline;
    action_header.baseline_id = player->___u26.server_update_data.action_baseline_id;

    remote_player_action_update_network_data action_body;
    memcpy(&action_body, &player->___u26.server_update_data.remote_player_action_update_baseline, sizeof(action_body));

    int size = message_delta_processor_encode_stateless(_message_remote_player_action_update, &action_header,
            &action_body, g_message_encode_buffer, 32760);
    network_game_server_send_message_to_machine(global_network_game_server_get(), machine_index,
            network_message_type_message_delta, g_message_encode_buffer, size, 1, 0, 0, 1);

    remote_player_position_update_header position_header;
    position_header.player_index = field_translated_index_translate_index(&field_properties_player_index_definition,
            existing_player_index);
    position_header.update_id = player->___u26.server_update_data.update_id_for_position_baseline;
    position_header.sequence_number = 0;

    remote_player_position_update_network_data position_body;
    memcpy(&position_body, &player->___u26.server_update_data.remote_player_position_update_baseline, sizeof(position_body));

    size = message_delta_processor_encode_stateless(_message_remote_player_position_update, &position_header,
            &position_body, g_message_encode_buffer, 32760);
    network_game_server_send_message_to_machine(global_network_game_server_get(), machine_index,
            network_message_type_message_delta, g_message_encode_buffer, size, 1, 0, 0, 1);

    remote_player_position_update_header vehicle_header;
    vehicle_header.player_index = field_translated_index_translate_index(&field_properties_player_index_definition,
            existing_player_index);
    vehicle_header.update_id = player->___u26.server_update_data.update_id_for_position_baseline;
    vehicle_header.sequence_number = 0;

    remote_player_vehicle_update_network_data vehicle_body;
    /* recovered: baseline passed by value -> &baseline (member is the typed struct, not a buffer) */
    memcpy(&vehicle_body, &player->___u26.server_update_data.remote_player_vehicle_update_baseline, sizeof(vehicle_body));

    size = message_delta_processor_encode_stateless(_message_remote_player_vehicle_update, &vehicle_header,
            &vehicle_body, g_message_encode_buffer, 32760);
    network_game_server_send_message_to_machine(global_network_game_server_get(), machine_index,
            network_message_type_message_delta, g_message_encode_buffer, size, 1, 0, 0, 1);
}
