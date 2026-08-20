/* default_locality_reference_position_encoder @0x8379CC68 — the default network encoder for a "locality"
 * reference position field (a world-space xyz). When a baseline exists and the move since it is small enough, it
 * delta-encodes: it writes a 0 bit, then for each axis a sign bit and the |delta|/cutoff value quantized to
 * gLOCALITY_BITS_PER_COMPONENT_DELTA_INTERNET bits. If there is no baseline, the move is too large, or the game
 * is not in the internet encoding class, it full-encodes: (a leading 1 bit only when a baseline existed) then
 * each axis's absolute position mapped from the [-5000, 5000] world range into
 * gLOCALITY_BITS_PER_COMPONENT_FULL_* bits. Moves below gLOCALITY_MINIMUM_MOVE_DISTANCE_INTERNET emit nothing.
 * Returns the number of bits written.
 *
 * DEVIATION: Hex-Rays punned several loads. The full-range base is the float constant -5000.0 (0xC59C4000),
 * which it rendered as `*(float*)(HIDWORD(v27) - 17296)` off a constant-pool base register; the 1e-4 factor makes
 * the world span 10000 units. The quantization multiplier is `(float)((1<<bits)-1)` (the `__PAIR64__` string and
 * `LODWORD(v27)=v27-1` are that max-value computation). Each quantized value is written through a temporary the
 * decompiler placed in HIDWORD(v37); reproduced as a plain unsigned int (big-endian, so &value is the field). */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"
#include "headers/message_delta_encoding_class.h"
#include <math.h>

#include "headers/message_delta_encoding_class.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern unsigned int bitstream_write_bit(bitstream_t *bit_stream, const unsigned int value);
extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int default_locality_reference_position_encoder(
        const _field_properties_definition *const field_properties, const void *const baseline_data,
        const void *const source_data, bitstream_t *const output_stream)
{
    const float *source = (const float *)source_data;
    const float *baseline = (const float *)baseline_data;

    unsigned int bits_written = 0;
    uint8_t use_full_encoding = 1;

    if ( baseline )
    {
        float delta[3];
        delta[0] = source[0] - baseline[0];
        delta[1] = source[1] - baseline[1];
        delta[2] = source[2] - baseline[2];

        if ( __fsqrts(delta[0] * delta[0] + (delta[2] * delta[2] + delta[1] * delta[1]))
                <= gLOCALITY_MINIMUM_MOVE_DISTANCE_INTERNET )
        {
            use_full_encoding = 0;
        }
        else
        {
            uint8_t sign[3];
            char exceeds_cutoff = 0;
            for ( int i = 0; i < 3; ++i )
            {
                sign[i] = 0;
                if ( delta[i] < 0.0f )
                {
                    delta[i] = -delta[i];
                    sign[i] = 1;
                }
                if ( delta[i] > gLOCALITY_DELTA_CUTOFF_DISTANCE_INTERNET )
                    exceeds_cutoff = 1;
            }

            if ( exceeds_cutoff || gMessageEncodingClass != _message_delta_encoding_class_internet )
            {
                bits_written = bitstream_write_bit(output_stream, 1);
                use_full_encoding = 1;
            }
            else
            {
                use_full_encoding = 0;
                bits_written = bitstream_write_bit(output_stream, 0);
                unsigned int delta_max = (1 << gLOCALITY_BITS_PER_COMPONENT_DELTA_INTERNET) - 1;
                for ( int i = 0; i < 3; ++i )
                {
                    bits_written += bitstream_write_bit(output_stream, sign[i]);
                    unsigned int quantized = (int)floor(
                            delta[i] / gLOCALITY_DELTA_CUTOFF_DISTANCE_INTERNET * (float)delta_max + 0.5);
                    if ( quantized > delta_max )
                        quantized = delta_max;
                    bits_written += bitstream_write_bits(output_stream, &quantized,
                            gLOCALITY_BITS_PER_COMPONENT_DELTA_INTERNET);
                }
            }
        }
    }

    if ( use_full_encoding )
    {
        int bits = gMessageEncodingClass ? gLOCALITY_BITS_PER_COMPONENT_FULL_INTERNET
                                         : gLOCALITY_BITS_PER_COMPONENT_FULL_LAN;
        unsigned int full_max = (1 << bits) - 1;
        for ( int i = 0; i < 3; ++i )
        {
            unsigned int quantized = (int)floor(
                    (float)full_max * ((source[i] - -5000.0f) * 0.000099999997f) + 0.5);
            if ( quantized > full_max )
                quantized = full_max;
            bits_written += bitstream_write_bits(output_stream, &quantized, bits);
        }
    }

    return bits_written;
}
