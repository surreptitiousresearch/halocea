/* default_vector_encoder @0x8379C080 — tail-call forwarder (`b default_point_encoder`; confirmed via
 * disasm, not a bare blr). Same reasoning as default_vector_decoder: a "vector" field encodes
 * identically to a "point" field. default_point_encoder itself is not yet decompiled — declared
 * extern with its own DB prototype and called through with the same arguments. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern unsigned int default_point_encoder(const _field_properties_definition *const field_properties, const void *const baseline_data, const void *const source_data, bitstream_t *const output_stream);

unsigned int default_vector_encoder(const _field_properties_definition *const field_properties,
        const void *const baseline_data, const void *const source_data, bitstream_t *const output_stream)
{
    return default_point_encoder(field_properties, baseline_data, source_data, output_stream);
}
