/* unit_exit_seat_from_network @0x836D64F8 — (client only) apply a "unit exit seat" message-delta: resolve
 * the network object index; when the message's force flag byte is set, finish the exit immediately,
 * otherwise attempt the normal seat exit and fall back to the immediate finish if it is refused. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/blam_data_globals.h"

typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);

extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t unit_try_and_exit_seat(int object_index, uint8_t should_allow_clients);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);

void unit_exit_seat_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    struct
    {
        int  translated_unit_index;
        char forced;
    } message;
    if ( !message_delta_processor_decode_stateless(&message, header) )
        return;

    int unit_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
                                                            message.translated_unit_index);
    if ( unit_index == -1 )
        return;

    if ( message.forced == 1 )
        unit_exit_seat_end(unit_index, 0, 1u, 0);
    else if ( !unit_try_and_exit_seat(unit_index, 1u) )
        unit_exit_seat_end(unit_index, 0, 1u, 0);
}
