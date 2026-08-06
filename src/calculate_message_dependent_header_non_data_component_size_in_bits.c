#include "headers/iterated_message.h"
#include "headers/message_definition.h"

/* DEVIATION: 'message_delta_processor_message_definition_type' is used as the index enum;
 * message_delta_global_message_list[] is a pointer array. body_fields.count is the first
 * field of _message_definition_field_reference_set (offset 0, 4 bytes). */
extern _message_definition *const message_delta_global_message_list[47];

int calculate_message_dependent_header_non_data_component_size_in_bits(message_delta_processor_message_definition_type definition_type)
{
    return message_delta_global_message_list[definition_type]->body_fields.count;
}
