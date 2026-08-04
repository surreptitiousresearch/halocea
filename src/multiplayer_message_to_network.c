/* multiplayer_message_to_network @0x83747220 — encode a "multiplayer HUD message" delta (translating the
 * recipient player index) and, if it encoded successfully, fetch the network game server (send is a no-op
 * here beyond that fetch — matches the decompiler exactly, which never uses the returned server pointer). */

#include "headers/field_properties_definition.h"
#include "headers/network_game_server.h"
#include "headers/blam_data_globals.h"
#include "headers/message_delta_message_ids.h"

typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);

void multiplayer_message_to_network(int recipient_player_index, int subject_player_index, int message,
    int message_data)
{
    int body[3];

    body[0] = field_translated_index_translate_index(&field_properties_player_index_definition, recipient_player_index);
    body[1] = subject_player_index;
    body[2] = message;

    if ( message_delta_processor_encode_stateless(
            _message_multiplayer_hud_message, 0, body, g_message_encode_buffer, 32760) > 0 )
    {
        global_network_game_server_get();
    }
}
