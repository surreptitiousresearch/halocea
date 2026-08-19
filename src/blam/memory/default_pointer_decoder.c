/* default_pointer_decoder @0x8379B768 — decode counterpart of default_pointer_encoder; see its DEVIATION
 * note for the nested-definition dispatch and by-value scalar smuggling through the void* slots. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

int default_pointer_decoder(const _field_properties_definition *const field_properties,
        const int *const baseline_data, const unsigned int *const destination_data, bitstream_t *const input_stream)
{
    _field_properties_definition *nested = *(_field_properties_definition **)field_properties->parameters;
    void *baseline_value = baseline_data ? (void *)(unsigned int)*baseline_data : 0;
    return nested->decode(nested, baseline_value, (void *)*destination_data, (void *)input_stream);
}
