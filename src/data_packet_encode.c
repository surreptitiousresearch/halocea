/* data_packet_encode @0x838200E8 — public entry point to serialize one packet: lazily compute the field sizes
 * on first use, set up the encoding state, emit a leading version byte (when the definition is versioned), then
 * recurse over the fields. Reports the encoded byte count and whether it fit. Returns no-overflow.
 *
 * The two recursion helpers are reached through leading-underscore symbols (_data_packet_verify /
 * _data_packet_encode); see those translation units. */

#include <stdint.h>
#include "headers/data_packet_definition.h"
#include "headers/data_encoding_state.h"

#include "headers/data_packet_field.h"
#include "headers/data_packet_field.h"
#include "headers/byte_swap_definition.h"
extern void _data_packet_verify(const data_packet_definition *packet_definition, int16_t *byte_count_reference, data_packet_field *first_field, int16_t *field_count_reference);
extern void data_encode_new(data_encoding_state *state, void *buffer, int buffer_size);
extern int data_encode_memory(data_encoding_state *state, const void *buffer, int16_t count, int code);
extern void _data_packet_encode(data_packet_definition *packet_definition, data_encoding_state *encode_state, int16_t version, int16_t *original_buffer, int16_t *byte_count_reference, data_packet_field *first_field, int16_t *field_count_reference);

uint8_t data_packet_encode(data_packet_definition *packet_definition, int16_t version, void *packet,
                                   void *buffer, int16_t *buffer_size, int16_t maximum_buffer_size)
{
    int16_t scratch_field_count;
    int16_t total_size_scratch[7];
    data_encoding_state state;

    if ( !packet_definition->initialized_flag )
    {
        _data_packet_verify(packet_definition, total_size_scratch, packet_definition->fields, &scratch_field_count);
        packet_definition->initialized_flag = 1;
    }

    data_encode_new(&state, buffer, maximum_buffer_size);
    if ( version == -1 )
        version = packet_definition->version;
    if ( packet_definition->version > 0 )
    {
        int16_t version_field;
        *(char *)&version_field = version; /* leading byte (big-endian) carries the version */
        data_encode_memory(&state, &version_field, 1, _1byte);
    }
    _data_packet_encode(packet_definition, &state, version, packet, 0, packet_definition->fields, 0);

    uint8_t result = state.overflow_flag == 0;
    *buffer_size = state.offset;
    return result;
}
