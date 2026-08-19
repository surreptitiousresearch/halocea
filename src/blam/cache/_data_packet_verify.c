/* _data_packet_verify @0x8381F8E8 — precompute each field's decoded byte stride (.size) and the packet's total
 * decoded size by walking the field list (terminated by type 9). Nested arrays (type 7) recurse to size their
 * sub-field list. A field outside the definition's version range keeps the previous computed size.
 *
 * Note: invoked through the leading-underscore symbol; Hex-Rays labels it (and its recursive call) without the
 * underscore. The initial `count` is an uninitialized stack value in the binary; initialized to 0 here. */

#include <stdint.h>
#include "headers/data_packet_definition.h"
#include "headers/data_packet_field.h"
#include "headers/packet_field_type.h"

void _data_packet_verify(const data_packet_definition *packet_definition, int16_t *byte_count_reference,
                         data_packet_field *first_field, int16_t *field_count_reference)
{
    data_packet_field *field = first_field;
    int16_t total_size = 0;
    int16_t count = 0;

    if ( first_field->type != __pack_end )
    {
        do
        {
            int version = packet_definition->version;
            if ( version >= field->first_version
              && (version <= field->last_version || !field->last_version) )
            {
                switch ( field->type )
                {
                    case __pack_pad:
                    case __pack_char:
                    case __pack_fixed_data: count = field->count;        break;
                    case __pack_short:      count = 2 * field->count;    break;
                    case __pack_long:       count = 4 * field->count;    break;
                    case __pack_int64:      count = 8 * field->count;    break;
                    case __pack_string:     count = field->count + 1;    break;  /* + null terminator */
                    case __pack_data:       count = field->count + 2;    break;  /* + 2-byte length prefix */
                    case __pack_array:
                    {
                        int16_t element_size = 0;
                        int16_t subfield_count = 0;
                        _data_packet_verify(packet_definition, &element_size, field + 1, &subfield_count);
                        int16_t array_bytes = field->count * element_size;
                        field += subfield_count;
                        count = array_bytes + 2;
                        break;
                    }
                    case __pack_end: count = 0; break;
                    default: break;
                }
            }
            field->size = count;
            ++field;
            total_size += count;
        }
        while ( field->type != __pack_end );
    }

    if ( field_count_reference )
        *field_count_reference = field - first_field + 1;
    if ( byte_count_reference )
        *byte_count_reference = total_size;
}
