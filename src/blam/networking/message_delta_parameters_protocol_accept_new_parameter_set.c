/* message_delta_parameters_protocol_accept_new_parameter_set @0x838122A0 — client-side handling of an
 * incoming parameter-set change message: discard it if still in legacy mode, otherwise decode the new
 * protocol number and unpack the transmitted parameter set. `client` is unused (matches disasm — never
 * referenced). */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/packed_parameter.h"
#include "headers/blam_data_globals.h"

extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern void unpack_parameter_set_from_transmission(packed_parameter *params);

void message_delta_parameters_protocol_accept_new_parameter_set(message_delta_processor_header *header,
    network_game_client *client)
{
    if ( g_protocol_changeover_enabled == 1 )
    {
        if ( header->decoding_information->mode )
        {
            message_delta_processor_discard_iteration_body(header);
        }
        else
        {
            unsigned char decoded[4];

            if ( message_delta_processor_decode_stateless(decoded, header) == 1 )
            {
                packed_parameter params;

                unpack_parameter_set_from_transmission(&params);
                g_parameters_protocol_number = decoded[0];
            }
        }
    }
}
