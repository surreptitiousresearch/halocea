/* default_flags_encoder @0x8379C118 — encode a flags dword bit-by-bit against an optional baseline: the
 * field's parameters blob is {int flag_count; unsigned char always_encode[flag_count];} at
 * field_properties->parameters. Only bits whose always_encode byte is 1 are considered. If no baseline is
 * given, or any considered bit differs from the baseline (or a prior bit already forced a write this call),
 * every considered bit is written; otherwise nothing is written (stream rewound to where it started) and
 * the function reports 0 bits written. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern unsigned int bitstream_tell(const bitstream_t *bit_stream);
extern unsigned int bitstream_write_bit(bitstream_t *bit_stream, const unsigned int value);
extern uint8_t bitstream_seek(bitstream_t *const bit_stream, const int offset, const bitstream_seek_origin origin);

int default_flags_encoder(const _field_properties_definition *const field_properties, unsigned int *baseline_data,
        int *source_data, bitstream_t *const output_stream)
{
    int flags = *source_data;
    int bits_written = 0;
    const unsigned int *parameters = field_properties->parameters;
    unsigned int start_position = bitstream_tell(output_stream);
    unsigned __int8 must_write = 0;

    for ( int i = 0; i < *parameters; ++i )
    {
        if ( *((const unsigned char *)parameters + i + 4) == 1 )
        {
            if ( !baseline_data || must_write
              || (((*baseline_data & (1 << i)) != 0) != (((1 << i) & flags) != 0)) )
                must_write = 1;
            bits_written += bitstream_write_bit(output_stream, ((1 << i) & flags) != 0);
        }
    }

    if ( must_write )
        return bits_written;

    bitstream_seek(output_stream, start_position, _bitstream_seek_beginning);
    return 0;
}
