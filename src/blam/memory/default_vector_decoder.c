/* default_vector_decoder @0x8379C088 — tail-call forwarder (`b default_point_decoder`; confirmed via
 * disasm, not a bare blr). A "vector" field decodes identically to a "point" field (both are 3-float
 * shapes), so the retail build simply reused default_point_decoder's body instead of compiling a
 * duplicate. default_point_decoder itself is not yet decompiled — declared extern with its own DB
 * prototype and called through with the same arguments (a raw `b` preserves all registers). */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int default_point_decoder(const _field_properties_definition *const field_properties, const void *const baseline_data, void *destination_data, bitstream_t *const input_stream);

int default_vector_decoder(const _field_properties_definition *const field_properties,
        const void *const baseline_data, void *destination_data, bitstream_t *const input_stream)
{
    return default_point_decoder(field_properties, baseline_data, destination_data, input_stream);
}
