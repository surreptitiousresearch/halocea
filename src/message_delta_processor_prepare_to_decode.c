/* message_delta_processor_prepare_to_decode @ 0x837A23C0 — Blam engine.
 * Reads the iteration-independent header from input_stream to prime an
 * incremental decode. On a valid header it records the decode state, stream, and
 * start position and returns 1; otherwise it rewinds the stream, marks the decode
 * stateless, and returns 0. */

#include <stdint.h>
#include "headers/message_delta_processor_decoding_information.h"

extern unsigned int bitstream_tell(const bitstream_t *bit_stream);
extern uint8_t bitstream_seek(bitstream_t *const bit_stream, const int offset, const bitstream_seek_origin origin);
extern int decode_iteration_independent_header(bitstream_t *const input_stream, message_delta_processor_decoding_information *const decoding_information);

int message_delta_processor_prepare_to_decode(bitstream_t *const input_stream, message_delta_processor_decoding_information *const decoding_information)
{
    unsigned int start_position = bitstream_tell(input_stream);
    int state = decode_iteration_independent_header(input_stream, decoding_information);

    if (state <= 0)
    {
        decoding_information->mode = _message_delta_mode_stateless;
        bitstream_seek(input_stream, start_position, _bitstream_seek_beginning);
        return 0;
    }

    decoding_information->state = state;
    decoding_information->input_stream = input_stream;
    decoding_information->original_stream_position = start_position;
    decoding_information->current_iteration = 1;
    return 1;
}
