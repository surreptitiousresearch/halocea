/* message_delta_processor_begin_decoding @0x837A08E0 — initialize a message-delta decode header: clear
 * the field-included bitmap, bind the decoding_information and custom-header destination, and reset the
 * iteration decode flags. Always returns true. */

#include "headers/message_delta_processor_header.h"

extern void *memset(void *dest, int value, unsigned int count);

int message_delta_processor_begin_decoding(message_delta_processor_decoding_information *const decoding_information, void *const custom_header_destination, message_delta_processor_header *const header)
{
    memset(header->field_included, 0, sizeof(header->field_included));
    header->decoding_information = decoding_information;
    header->custom_header = custom_header_destination;
    decoding_information->iteration_body_decoded = 0;
    decoding_information->iteration_header_decoded = 0;
    return 1;
}
