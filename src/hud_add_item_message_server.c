#include <stdint.h>
#include "headers/game_connection.h"
#include "headers/hud_add_item_message.h"
#include "headers/blam_data_globals.h"
/* hud_add_item_message_server @0x836A5C40 — show (and, on a server, replicate) a HUD item-pickup
 * message. On a non-networked or client session the message is shown locally; on a server it is encoded
 * as a message-delta to the clients.
 *
 * DEVIATION: after a successful encode the binary only calls global_network_game_server_get() and
 * discards the result (the actual send is folded/inlined elsewhere); reproduced verbatim. */

extern int16_t game_connection(void);
extern void hud_add_item_message_client(int16_t local_player_index, int item_definition_index, int16_t quantity, char message_offset);
/* deviation: was `extern int _message_hud_add_item;` — binary emits li r3,6 @0x836A5C64 (immediate),
 * so this is the enum constant _message_hud_add_item = 6 (headers_ref
 * message_delta_processor_message_definition_type), not a global load. Belongs in
 * headers/message_delta_message_ids.h — hoist there on propagation. */
#include "headers/message_delta_message_ids.h"
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
typedef struct network_game_server network_game_server;
extern network_game_server *global_network_game_server_get(void);

void hud_add_item_message_server(int16_t local_player_index, int8_t machine_index, int item_definition_index, int16_t quantity, uint8_t message_offset)
{
    if ( !game_connection() || game_connection() == _game_connection_film_playback )
    {
        hud_add_item_message_client(local_player_index, item_definition_index, quantity, message_offset);
    }
    else
    {
        hud_add_item_message message;
        message.item_definition_index = item_definition_index;
        message.message_offset = message_offset;
        message.quantity = quantity;
        if ( message_delta_processor_encode_stateless(_message_hud_add_item, 0, &message, g_message_encode_buffer, 32760) > 0 )
            global_network_game_server_get();
    }
}
