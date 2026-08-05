/* _data_packet_decode @0x8381FDAC — recursive per-field packet decoder, the counterpart to
 * `_data_packet_encode`. Walks a field list (terminated by a type-9 field), decoding each field whose version
 * range includes `version` into the decoded buffer; fields outside the version range are zero-filled instead.
 * Nested arrays (type 7) recurse over the following sub-field list. Reports the consumed field count and
 * decoded byte span through the *_reference outparams.
 *
 * Note: the public-facing wrapper (`data_packet_decode`) calls this via the leading-underscore symbol; the
 * Hex-Rays output labels both this and its verify helper without the underscore (see `_data_packet_verify` @
 * 0x8381F8E8). Disasm-confirmed clean: the type-7 recursive call and the `_data_packet_verify` call both match
 * their real signatures exactly, no argument scrambling here (unlike the thin wrapper). */

#include <stdint.h>
#include <string.h>
#include "headers/data_packet_definition.h"
#include "headers/data_packet_field.h"
#include "headers/data_encoding_state.h"
#include "headers/packet_field_type.h"

extern char *data_decode_memory(data_encoding_state *state, int16_t count, int code);
extern char * data_decode_string(data_encoding_state *state, int16_t maximum_string_length);
extern int16_t data_decode_integer(data_encoding_state *state, int maximum_value);
extern void _data_packet_verify(const data_packet_definition *packet_definition, int16_t *byte_count_reference, data_packet_field *first_field, int16_t *field_count_reference);

void _data_packet_decode(data_packet_definition *packet_definition, data_encoding_state *decode_state,
                         int16_t version, int16_t *original_buffer, int16_t *byte_count_reference,
                         data_packet_field *first_field, int16_t *field_count_reference)
{
    int16_t buffer_start = (int16_t)(int)original_buffer;
    data_packet_field *field = first_field;
    int16_t *cursor = original_buffer;

    if ( first_field->type != __pack_end )
    {
        do
        {
            if ( version < field->first_version || (version > field->last_version && field->last_version) )
            {
                /* field is absent in this version: zero-fill its decoded slot */
                memset(cursor, 0, field->size);
            }
            else if ( field->type >= __pack_char && field->type <= __pack_fixed_data )
            {
                switch ( field->type )
                {
                    case __pack_short:
                    {
                        void *source = data_decode_memory(decode_state, field->count, -2);
                        if ( source )
                            memcpy(cursor, source, 2 * field->count);
                        break;
                    }
                    case __pack_long:
                    {
                        void *source = data_decode_memory(decode_state, field->count, -4);
                        if ( source )
                            memcpy(cursor, source, 4 * field->count);
                        break;
                    }
                    case __pack_int64:
                    {
                        void *source = data_decode_memory(decode_state, field->count, -8);
                        if ( source )
                            memcpy(cursor, source, 8 * field->count);
                        break;
                    }
                    case __pack_string:
                    {
                        char *source = data_decode_string(decode_state, field->count);
                        if ( source )
                        {
                            char *dest = (char *)cursor;
                            char c;
                            do
                            {
                                c = *source;
                                *dest = c;
                                source++;
                                dest++;
                            }
                            while ( c );
                        }
                        break;
                    }
                    case __pack_data:
                    {
                        int16_t array_count = data_decode_integer(decode_state, field->count);
                        *cursor = array_count;
                        void *source = data_decode_memory(decode_state, array_count, 1);
                        if ( source )
                            memcpy(cursor + 1, source, array_count);
                        break;
                    }
                    case __pack_array:
                    {
                        int16_t array_count = data_decode_integer(decode_state, field->count);
                        int16_t subfield_count = 0;
                        _data_packet_verify(packet_definition, 0, field + 1, &subfield_count);
                        if ( array_count < 0 || array_count > field->count )
                            array_count = 0;
                        *cursor = array_count;
                        char *element = (char *)(cursor + 1);
                        for ( int16_t e = 0; e < array_count; e = (int16_t)(e + 1) )
                        {
                            int16_t element_bytes = 0;
                            _data_packet_decode(packet_definition, decode_state, version,
                                                (int16_t *)element, &element_bytes, field + 1, 0);
                            element += element_bytes;
                        }
                        field += subfield_count;
                        break;
                    }
                    default:
                    {
                        void *source = data_decode_memory(decode_state, field->count, 1);
                        if ( source )
                            memcpy(cursor, source, field->count);
                        break;
                    }
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
        *byte_count_reference = (int16_t)(int)cursor - buffer_start;
}
