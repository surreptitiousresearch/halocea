/* multiplayer_message_from_network @0x83751528 — decodes a stateless multiplayer-message-delta and routes
 * it to the local player: translates the network object index back to a local player index and forwards
 * (message, message_data) via multiplayer_message. Mirrors multiplayer_sound_from_network.c's
 * discard-iteration-body-if-incremental / decode-stateless-otherwise structure. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/field_properties_definition.h"
#include "headers/blam_data_globals.h"


extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern int local_player_get_player_index(int16_t local_player_index);
extern void multiplayer_message(int recipient_player_index, int subject_player_index, int message, int message_data, uint8_t should_replicate);

void multiplayer_message_from_network(message_delta_processor_header *header, network_game_client *client)
{
    struct
    {
        int network_player_index;
        int message;
        int message_data;
    } body;

    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
    }
    else if ( message_delta_processor_decode_stateless(&body, header) )
    {
        int recipient_player_index = local_player_get_player_index(0);
        int subject_player_index = field_translated_index_get_local_index(&field_properties_player_index_definition,
            body.network_player_index);
        multiplayer_message(recipient_player_index, subject_player_index, body.message, body.message_data, 0);
    }
}
