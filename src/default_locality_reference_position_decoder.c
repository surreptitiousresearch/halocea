/* default_locality_reference_position_decoder @0x8379CF10 — decode a networked "locality reference position"
 * (a world-space 3-vector) from a bitstream. When a baseline is supplied a leading bit chooses between a full
 * re-send and a per-component signed delta: each component reads a sign bit plus a magnitude quantized to
 * gLOCALITY_BITS_PER_COMPONENT_DELTA_INTERNET bits scaled over gLOCALITY_DELTA_CUTOFF_DISTANCE_INTERNET, added
 * to the baseline. A full send (also forced when there is no baseline) reads each component at the
 * class-dependent full bit width (Internet vs LAN) and maps it linearly onto [-5000, 5000]. Returns the number
 * of bits read.
 *
 * DEVIATION: the DB prototype is 4 args; Hex-Rays invented phantom a5/a6/a7 and modeled the int->float
 * conversions and the (1<<bits)-1 quantization divisor as __int128/__int64 register puns — reconstructed as
 * plain integer math (the full-send path uses the identical (1<<bits)-1 divisor in clean form). The
 * field_properties argument is unused by this decoder. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

#include "headers/message_delta_encoding_class.h"
#include "headers/blam_data_globals.h"

extern unsigned int bitstream_read_bit(bitstream_t *const bit_stream, uint8_t *const value);
extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

int default_locality_reference_position_decoder(const _field_properties_definition *field_properties, const float *baseline_data, float *destination_data, bitstream_t *input_stream)
{
    unsigned int bits_read = 0;
    uint8_t full_update = 0;

    if ( baseline_data )
    {
        bits_read = bitstream_read_bit(input_stream, &full_update);
        if ( !full_update )
        {
            float deltas[3];
            for ( int i = 0; i < 3; i++ )
            {
                unsigned int magnitude = 0;
                uint8_t sign;
                bits_read += bitstream_read_bit(input_stream, &sign);
                bits_read += bitstream_read_bits(input_stream, &magnitude, gLOCALITY_BITS_PER_COMPONENT_DELTA_INTERNET);
                float value = (float)magnitude / (float)((1 << gLOCALITY_BITS_PER_COMPONENT_DELTA_INTERNET) - 1)
                            * gLOCALITY_DELTA_CUTOFF_DISTANCE_INTERNET;
                deltas[i] = sign ? -value : value;
            }
            destination_data[0] = baseline_data[0] + deltas[0];
            destination_data[1] = baseline_data[1] + deltas[1];
            destination_data[2] = baseline_data[2] + deltas[2];
        }
    }
    else
    {
        full_update = 1;
    }

    if ( full_update )
    {
        int component_bits = gMessageEncodingClass
            ? gLOCALITY_BITS_PER_COMPONENT_FULL_INTERNET
            : gLOCALITY_BITS_PER_COMPONENT_FULL_LAN;
        unsigned int max_value = (1u << component_bits) - 1;
        for ( int i = 0; i < 3; i++ )
        {
            unsigned int raw = 0;
            bits_read += bitstream_read_bits(input_stream, &raw, component_bits);
            destination_data[i] = (float)raw / (float)max_value * 10000.0f - 5000.0f;
        }
    }

    return bits_read;
}
