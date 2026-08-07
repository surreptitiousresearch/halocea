/* _data_packet_encode @0x8381FAA0 — recursive per-field packet encoder. Walks a field list (terminated by a
 * type-9 field), encoding each field whose version range includes `version` from the decoded buffer; fields
 * outside the version range emit a zero/default placeholder so the wire layout stays version-stable. Nested
 * arrays (type 7) recurse over the following sub-field list. Reports the consumed field count and decoded byte
 * span through the *_reference outparams.
 *
 * Note: the public-facing wrapper calls this via the leading-underscore symbol; the Hex-Rays output labels
 * both this and its verify helper without the underscore (see _data_packet_verify @0x8381F8E8). */

#include <stdint.h>
#include "headers/data_packet_definition.h"
#include "headers/data_packet_field.h"
#include "headers/data_encoding_state.h"
#include "headers/packet_field_type.h"

#include "headers/byte_swap_definition.h"
extern int data_encode_memory(data_encoding_state *state, const void *buffer, int16_t count, int code);
extern int data_encode_string(data_encoding_state *state, const char *source_string, int16_t maximum_string_length);
extern int data_encode_integer(data_encoding_state *state, int value, int maximum_value);
extern void _data_packet_verify(const data_packet_definition *packet_definition, int16_t *byte_count_reference, data_packet_field *first_field, int16_t *field_count_reference);

void _data_packet_encode(data_packet_definition *packet_definition, data_encoding_state *encode_state,
                         int16_t version, int16_t *original_buffer, int16_t *byte_count_reference,
                         data_packet_field *first_field, int16_t *field_count_reference)
{
    int16_t buffer_start = (int16_t)(uintptr_t)original_buffer;
    data_packet_field *field = first_field;
    int16_t *cursor = original_buffer;

    if ( first_field->type != __pack_end )
    {
        do
        {
            if ( version >= field->first_version
              && (version <= field->last_version || !field->last_version) )
            {
                /* field is live in this version: encode its real value */
                if ( field->type >= __pack_char && field->type <= __pack_fixed_data )
                {
                    switch ( field->type )
                    {
                        case __pack_short: data_encode_memory(encode_state, cursor, field->count, _2byte); break;
                        case __pack_long: data_encode_memory(encode_state, cursor, field->count, _4byte); break;
                        case __pack_int64: data_encode_memory(encode_state, cursor, field->count, _8byte); break;
                        case __pack_string: data_encode_string(encode_state, (char *)cursor, field->count); break;
                        case __pack_data:
                        {
                            int16_t array_count = *cursor;
                            if ( *cursor < 0 || array_count > field->count )
                                array_count = 0;
                            data_encode_integer(encode_state, array_count, field->count);
                            data_encode_memory(encode_state, cursor + 1, array_count, _1byte);
                            break;
                        }
                        case __pack_array:
                        {
                            int16_t array_count = *cursor;
                            char *element = (char *)(cursor + 1);
                            int16_t subfield_count = 0;
                            _data_packet_verify(packet_definition, 0, field + 1, &subfield_count);
                            if ( array_count < 0 || array_count > field->count )
                                array_count = 0;
                            data_encode_integer(encode_state, array_count, field->count);
                            for ( int16_t e = 0; e < array_count; e = (int16_t)(e + 1) )
                            {
                                int16_t element_bytes = 0;
                                _data_packet_encode(packet_definition, encode_state, version,
                                                    (int16_t *)element, &element_bytes, field + 1, 0);
                                element += element_bytes;
                            }
                            field += subfield_count;
                            break;
                        }
                        default: data_encode_memory(encode_state, cursor, field->count, _1byte); break;
                    }
                }
            }
            else if ( field->type >= __pack_char && field->type <= __pack_fixed_data )
            {
                /* field absent in this version: emit a zero/default placeholder of the right width */
                if ( field->type == __pack_short || field->type == __pack_long || field->type == __pack_int64 )
                {
                    data_encode_memory(encode_state, 0, field->count, _1byte);
                }
                else if ( field->type != __pack_string )
                {
                    if ( field->type == __pack_data || field->type == __pack_array )
                        data_encode_integer(encode_state, 0, field->count);
                    else
                        data_encode_memory(encode_state, 0, field->count, _1byte);
                }
                else
                {
                    char terminator = 0;
                    data_encode_memory(encode_state, &terminator, 1, _1byte);
                }
            }

            int16_t stride = field->size;
            ++field;
            cursor = (int16_t *)((char *)cursor + stride);
        }
        while ( field->type != __pack_end );
    }

    if ( field_count_reference )
        *field_count_reference = field - first_field + 1;
    if ( byte_count_reference )
        *byte_count_reference = (int16_t)(uintptr_t)cursor - buffer_start;
}
