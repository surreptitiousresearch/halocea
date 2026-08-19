/* default_pointer_encoder @0x8379B738 — generic encode callback for a field that indirects through
 * another field's definition. `*(_field_properties_definition **)field_properties->parameters` is the
 * nested definition; this delegates to *its* encode callback, passing itself as the definition argument
 * (verified via disasm: lwz r10,0x50(r11) reads the nested definition's `encode` member at the same
 * offset field_properties_definition.h already models, then tail-calls it — mtctr/bctr with r6 untouched,
 * so output_stream flows straight through).
 *
 * The baseline/source values are 4-byte scalars (an index or handle) rather than pointers-to-data; they are
 * forwarded through the nested callback's void* slots by value, which is how "pointer" fields smuggle a
 * plain handle through the generic encode signature. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

unsigned int default_pointer_encoder(const _field_properties_definition *const field_properties,
        const int *const baseline_data, const unsigned int *const source_data, bitstream_t *const output_stream)
{
    _field_properties_definition *nested = *(_field_properties_definition **)field_properties->parameters;
    void *baseline_value = baseline_data ? (void *)(unsigned int)*baseline_data : 0;
    return (unsigned int)nested->encode(nested, baseline_value, (void *)*source_data, (void *)output_stream);
}
