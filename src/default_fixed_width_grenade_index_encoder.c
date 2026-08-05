/* default_fixed_width_grenade_index_encoder @0x8379D4F8 — encode a grenade index change relative to
 * baseline (2 bits, same packing as encode_grenade_index.c — confirmed via disasm_range(0x8379D4F8,
 * 0x8379D560) as inlined, not a call, so the helper is duplicated per that file's established convention). */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

static unsigned int ppc_cntlzw(unsigned int x)
{
    if (x == 0) return 32;
    unsigned int n = 0;
    if ((x & 0xFFFF0000u) == 0) { n += 16; x <<= 16; }
    if ((x & 0xFF000000u) == 0) { n +=  8; x <<=  8; }
    if ((x & 0xF0000000u) == 0) { n +=  4; x <<=  4; }
    if ((x & 0xC0000000u) == 0) { n +=  2; x <<=  2; }
    if ((x & 0x80000000u) == 0) { n +=  1; }
    return n;
}

unsigned int default_fixed_width_grenade_index_encoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, const void *const source_data, bitstream_t *const output_stream)
{
    const int16_t *source = source_data;
    const int16_t *baseline = baseline_data;
    unsigned int encoded = (ppc_cntlzw((unsigned int)(*source + 1)) >> 4) & 2 | *source & 1;

    if ( !baseline || encoded != ((ppc_cntlzw((unsigned int)(*baseline + 1)) >> 4) & 2 | *baseline & 1) )
        return bitstream_write_bits(output_stream, &encoded, 2u);
    return 0;
}
