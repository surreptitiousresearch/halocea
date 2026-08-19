/* message_delta_processor_get_decoded_message_size @ 0x837A0A00 — Blam engine.
 * Returns the number of bits consumed from the input stream so far by the
 * in-progress decode (current position minus the position where the decode
 * started). */

#include "headers/message_delta_processor_header.h"

extern unsigned int bitstream_tell(const bitstream_t *bit_stream);

unsigned int message_delta_processor_get_decoded_message_size(const message_delta_processor_header *const header)
{
    message_delta_processor_decoding_information *decoding_information = header->decoding_information;
    return bitstream_tell(decoding_information->input_stream)
         - decoding_information->original_stream_position;
}
