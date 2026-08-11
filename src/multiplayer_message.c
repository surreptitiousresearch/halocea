/* multiplayer_message @0x8374D5B0 — present a multiplayer HUD/chat message to a recipient player and, on
 * the server, replicate it to clients. Builds the localized message text for the subject player; routes
 * chat-style messages (kill/quit/score notifications) to the chat box and the rest to the HUD print line.
 * When running as the server and replication is requested, the message is encoded as a stateless message
 * delta for transmission.
 *
 * DEVIATION: after a successful encode the disassembly only fetches the server pointer and discards it
 * (no send call follows in this build); reproduced faithfully. */

#include <wchar.h>
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/field_properties_definition.h"
#include "headers/game_connection.h"
#include "headers/game_engine_message.h"
#include "headers/blam_data_globals.h"


#include "headers/network_game_server.h"
#include "headers/message_delta_message_ids.h"
typedef struct _field_properties_definition _field_properties_definition;
extern void *datum_try_and_get(const data_array *data, int index);
extern uint8_t multiplayer_message_internal(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size);
extern void hud_chat_box_add_message(const wchar_t *message);
extern void hud_print_message(int16_t local_player_index, const wchar_t *text);
extern int16_t game_connection(void);
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);

void multiplayer_message(int recipient_player_index, int subject_player_index, int message,
                         int message_data, uint8_t should_replicate)
{
    uint16_t *recipient = datum_try_and_get(player_data, recipient_player_index);
    if (!recipient)
        return;

    uint16_t message_text[1048];
    if (recipient[1] != 0xFFFF &&
        multiplayer_message_internal(subject_player_index, message, message_data, message_text, 1024))
    {
        unsigned char is_chat_message;
        /* Chat-box-routed messages (welcome/kill/quit/score notifications) vs. HUD print line.
         * message ids from game_engine_message ($98767E2EB1F0D516F9DAD25AB06E7A9B). */
        switch (message)
        {
            case game_engine_message_welcome:
            case game_engine_message_killed_by_unknown:
            case game_engine_message_killed_by_biped:
            case game_engine_message_killed_by_vehicle:
            case game_engine_message_killed_by_player:
            case game_engine_message_killed_by_friendly_fire:
            case game_engine_message_killed_by_self:
            case game_engine_message_killed_enemy:
            case game_engine_message_killed_friendly:
            case game_engine_message_killed_enemy_with_score:
            case game_engine_message_player_quit_self:
            case game_engine_message_quit:
                is_chat_message = 1;
                break;
            default:
                is_chat_message = 0;
                break;
        }

        message_text[1023] = 0;
        if (is_chat_message)
            hud_chat_box_add_message(message_text);
        else
            hud_print_message(recipient[1], message_text);
    }

    if (game_connection() == _game_connection_network_server && should_replicate == 1)
    {
        int message_body[4];
        message_body[0] = field_translated_index_translate_index(&field_properties_player_index_definition,
                                                                 subject_player_index);
        message_body[1] = message;
        message_body[2] = message_data;
        if (message_delta_processor_encode_stateless(_message_multiplayer_hud_message, nullptr, message_body,
                                                     g_message_encode_buffer, 32760) > 0)
            global_network_game_server_get();
    }
}
