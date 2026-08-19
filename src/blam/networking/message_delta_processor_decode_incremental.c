/* message_delta_processor_decode_incremental @0x837A1C70 — decodes one incremental message body into
 * `destination_data` against `baseline_data`. Considered decoded if any bits were consumed OR the caller
 * allows an empty body; on success advances the decoding-information's state and marks the body decoded, on
 * failure rewinds the input stream back to its position when decoding began. Mirrors
 * message_delta_processor_decode_stateless.c's stateless counterpart, adding the baseline/allow-empty
 * handling. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"

#include "headers/bitstream_t.h"
extern int decode_message_body(message_delta_processor_decoding_information *const decoding_information, const uint8_t *const field_included, const char *baseline_data, char *destination_data); /* field_included/baseline const per disasm 0x837A1880 (read-only) 2026-07-31 (C4090) */
extern uint8_t bitstream_seek(bitstream_t *const bit_stream, const int offset, const bitstream_seek_origin origin);

uint8_t message_delta_processor_decode_incremental(void *const destination_data,
    const void *const baseline_data, const message_delta_processor_header *const header,
    const uint8_t allow_empty_body)
{
    message_delta_processor_decoding_information *decoding_information = header->decoding_information;
    int bits_consumed = decode_message_body(decoding_information, header->field_included, baseline_data, destination_data);

    decoding_information->state += bits_consumed;

    uint8_t body_decoded;
    if ( bits_consumed || allow_empty_body )
    {
        body_decoded = 1;
        decoding_information->iteration_body_decoded = 1;
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
