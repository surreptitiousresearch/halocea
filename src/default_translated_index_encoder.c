#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

/* default_translated_index_encoder @0x8379BA30 — the generic encode callback installed in the
 * _field_properties_definition.encode slot of the translated-index field types.
 * DEVIATION: the signature is the shared encode-slot contract
 *   int (*)(const _field_properties_definition *const, void *, void *, void *)
 * (DB types_members for _field_properties_definition +0x50), not the narrower
 * `unsigned int (..., unsigned int *, unsigned int *, bitstream_t *const)` the decompiler inferred
 * from this one body. The slot is shared by every field type, so the payload pointers are opaque
 * here and the datum width is a property of this encoder, not of the interface. */
int default_translated_index_encoder(const _field_properties_definition *const field_properties,
        void *baseline_data, void *source_data, void *output_stream)
{
    const unsigned int *baseline = (const unsigned int *)baseline_data;
    const unsigned int *source = (const unsigned int *)source_data;

    int changed = 1;
    if (baseline)
        changed = *baseline != *source;

    if (!changed)
        return 0;

    int bit_count = *((int *)field_properties->parameters + 2);
    return bitstream_write_bits((bitstream_t *)output_stream, source, bit_count);
}
