/* message_delta_processor_discard_iteration_body @0x837A1D10 — decode one iteration body into a throwaway
 * scratch buffer without exposing the result, just to advance the bitstream cursor past it (used to skip an
 * iteration a caller isn't interested in). In incremental mode, decodes against the same scratch buffer as
 * its own baseline (so delta-encoded fields still decode structurally) and always counts as decoded, even
 * with 0 bits consumed; in stateless mode at least one bit must be consumed. On failure, rewinds the input
 * stream to where decoding began. Mirrors message_delta_processor_decode_incremental.c's structure. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"

#include "headers/bitstream_t.h"
extern int decode_message_body(message_delta_processor_decoding_information *const decoding_information, const uint8_t *const field_included, const char *baseline_data, char *destination_data); /* field_included/baseline const per disasm 0x837A1880 (read-only) 2026-07-31 (C4090) */
extern uint8_t bitstream_seek(bitstream_t *const bit_stream, const int offset, const bitstream_seek_origin origin);

uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header)
{
    message_delta_processor_decoding_information *decoding_information = header->decoding_information;
    unsigned char scratch[2056];

    const void *baseline_data = decoding_information->mode != _message_delta_mode_incremental ? 0 : scratch;
    int bits_consumed = decode_message_body(decoding_information, header->field_included, baseline_data, scratch);

    uint8_t body_decoded;
    if ( bits_consumed || decoding_information->mode )
    {
        decoding_information->state += bits_consumed;
        decoding_information->iteration_body_decoded = 1;
        body_decoded = 1;
    }
    else
    {
        body_decoded = 0;
    }

    if ( !body_decoded )
        bitstream_seek(decoding_information->input_stream, decoding_information->original_stream_position,
            _bitstream_seek_beginning);

    return body_decoded;
}
