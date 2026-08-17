/* decode_header_field @0x837A0E60 */
#include "headers/message_definition.h"
#include "headers/bitstream_t.h"

extern _message_definition *const message_delta_global_message_list[47];

int decode_header_field(message_delta_processor_message_definition_type definition_type, int field_index,
                         char *destination_data, bitstream_t *const input_stream)
{
    _field_reference_definition *field_reference =
        &message_delta_global_message_list[definition_type]->header_fields->field_references[field_index];

    return field_reference->properties->decode(field_reference->properties, 0,
                                                 &destination_data[field_reference->offset], input_stream);
}
