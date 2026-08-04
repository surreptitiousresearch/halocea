/* message_definition_initialize @0x837A2A50 — one-time setup of a message-delta message definition:
 * initialize both field reference sets, compute their maximum data sizes, and derive the per-message bit
 * accounting: header size (header bits + one field-map bit per body field, the count fetched back through
 * the global message list as compiled), iteration size (header + field map + body), iteration-independent
 * overhead (7 bits — or the protocol packet size + 8 under protocol changeover — plus the bits needed to
 * express max_iterations), and the maximum total size. */

#include "headers/message_definition.h"
#include "headers/blam_data_globals.h"

typedef struct _message_definition_field_reference_set _message_definition_field_reference_set;
extern _message_definition **message_delta_global_message_list;
extern int bits_needed[];

extern int message_definition_field_reference_set_initialize(_message_definition_field_reference_set *const field_reference_set);
extern int message_definition_field_reference_set_calculate_maximum_size(const _message_definition_field_reference_set *const field_reference_set);
extern int message_delta_parameters_protocol_get_packet_bit_size(void);

void message_definition_initialize(_message_definition *const message_definition)
{
    message_definition_field_reference_set_initialize(message_definition->header_fields);
    message_definition_field_reference_set_initialize(&message_definition->body_fields);

    int body_size = message_definition_field_reference_set_calculate_maximum_size(
                        &message_definition->body_fields);
    int header_size = message_definition_field_reference_set_calculate_maximum_size(
                          message_definition->header_fields);
    int field_map_bits =
        message_delta_global_message_list[message_definition->definition_type]->body_fields.count;

    int independent_bits = 7;
    if ( g_protocol_changeover_enabled == 1 )
        independent_bits = message_delta_parameters_protocol_get_packet_bit_size() + 8;

    int iteration_count_bits = 0;
    if ( message_definition->max_iterations > 1 )
        iteration_count_bits = bits_needed[message_definition->max_iterations - 1];

    message_definition->body_fields.maximum_data_size = body_size;
    message_definition->header_fields->maximum_data_size = header_size;
    message_definition->header_size_in_bits = field_map_bits + header_size;
    message_definition->iteration_size_in_bits = field_map_bits + header_size + body_size;
    message_definition->iteration_independent_size_in_bits = iteration_count_bits + independent_bits;
    message_definition->initialized = 1;
    message_definition->maximum_total_size =
        message_definition->max_iterations * (field_map_bits + header_size + body_size)
      + iteration_count_bits + independent_bits;
}
