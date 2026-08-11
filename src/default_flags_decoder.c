/* default_flags_decoder @0x8379C210 — decodes a bitfield: for each bit position flagged "networked" in the
 * field's parameters blob (dword count at +0, per-bit usage bytes at +4), reads one bit from the stream and
 * sets/clears that bit in *destination_data. Returns the total bits consumed. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern unsigned int bitstream_read_bit(bitstream_t *const bit_stream, uint8_t *const value);

int default_flags_decoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, void *destination_data, bitstream_t *const input_stream)
{
    const int *parameters = field_properties->parameters;
    const unsigned char *bit_used = (const unsigned char *)&parameters[1]; /* usage bytes follow the count dword */
    int flags = *(int *)destination_data;
    int bits_read = 0;

    for ( int i = 0; i < *parameters; ++i )
    {
        if ( bit_used[i] == 1 )
        {
            unsigned char bit_value = 0;
            bits_read += bitstream_read_bit(input_stream, &bit_value);

            int bit = 1 << i;
            if ( bit_value )
                flags |= bit;
            else
                flags &= ~bit;
        }
    }

    *(int *)destination_data = flags;
    return bits_read;
}
