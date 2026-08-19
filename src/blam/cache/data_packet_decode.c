/* data_packet_decode @0x838201B8 — decode one packet from `encoded_packet` into `decoded_packet`. Lazily
 * verifies the packet definition on first use (via `_data_packet_verify`), then reads a version byte from the
 * stream when the definition is itself versioned (`packet_definition->version != 0`); if the encoded
 * version isn't newer than the definition's own version, decodes the field list via `_data_packet_decode` and
 * succeeds as long as the stream didn't overflow. Optionally reports the decoded version and the number of
 * bytes actually consumed from the stream.
 *
 * DEVIATION — the DB's decompile renders both `_data_packet_verify`/`_data_packet_decode` calls as recursive
 * self-calls with a mismatched argument count from this function's own signature; disasm_range
 * (0x838201B8-0x83820290) confirms both are calls to separate, larger functions (`_data_packet_verify` @
 * 436B, `_data_packet_decode` @ 772B) that merely share a display name with their thin wrappers. */

#include <stdint.h>
#include "headers/data_packet_definition.h"
#include "headers/data_encoding_state.h"

#include "headers/data_packet_field.h"
#include "headers/data_packet_field.h"
extern void _data_packet_verify(const data_packet_definition *packet_definition, int16_t *byte_count_reference, data_packet_field *first_field, int16_t *field_count_reference);
extern void _data_packet_decode(data_packet_definition *packet_definition, data_encoding_state *decode_state, int16_t version, int16_t *original_buffer, int16_t *byte_count_reference, data_packet_field *first_field, int16_t *field_count_reference);
extern void data_decode_new(data_encoding_state *state, void *buffer, int buffer_size);
extern uint8_t data_decode_byte(data_encoding_state *state);

uint8_t data_packet_decode(data_packet_definition *packet_definition, void *encoded_packet, int16_t encoded_packet_size,
        void *decoded_packet, int16_t *version_reference, int16_t *actual_encoded_packet_size_reference)
{
    uint8_t success = 0;

    if (!packet_definition->initialized_flag)
    {
        int16_t byte_count;
        int16_t field_count;
        _data_packet_verify(packet_definition, &byte_count, packet_definition->fields, &field_count);
        packet_definition->initialized_flag = 1;
    }

    data_encoding_state decode_state;
    data_decode_new(&decode_state, encoded_packet, encoded_packet_size);

    int16_t version = packet_definition->version;
    if (version)
        version = data_decode_byte(&decode_state);

    if (version <= packet_definition->version)
    {
        _data_packet_decode(packet_definition, &decode_state, version, decoded_packet, 0, packet_definition->fields, 0);
        if (!decode_state.overflow_flag)
            success = 1;
    }

    if (version_reference)
        *version_reference = version;

    if (actual_encoded_packet_size_reference)
        *actual_encoded_packet_size_reference = decode_state.offset;

    return success;
}
