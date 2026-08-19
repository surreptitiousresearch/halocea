/* convert_to_quantized_smart @0x8379D758 — quantize a 3-component vector against a smart_vector field type's
 * min/max range, into a fixed-width integer per component. Uses the LAN range/bit-width
 * (range_of_values_per_component_lan) unless gMessageEncodingClass is non-zero, in which case the narrower
 * internet range (range_of_values_per_component_internet) is used for all three components.
 *
 * DEVIATION: the decompiler rendered all three map_to_fixed_width calls with parameters->min_value/max_value
 * but garbled the 4th (range) argument — showing `(unsigned int)in` for x and the *previous component's own
 * quantized result* (out->x, out->y) for y and z. Disasm (r6 reload before each call, and the
 * gMessageEncodingClass branch selecting between parameters+0xC and parameters+0x14 once up front) confirms
 * all three components actually use the SAME range value, chosen once per call by gMessageEncodingClass —
 * not an accumulating/previous-result argument. */

#include "headers/real_vector3d.h"
#include "headers/field_type_smart_vector_parameters.h"

#include "headers/message_delta_encoding_class.h"
#include "headers/blam_data_globals.h"

extern unsigned int map_to_fixed_width(float input, float lowerBound, float upperBound, unsigned int range);

void convert_to_quantized_smart(const real_vector3d *const in, quantized_smart_vector *const out,
    const _field_type_smart_vector_parameters *const parameters)
{
    unsigned int range = gMessageEncodingClass
        ? parameters->range_of_values_per_component_internet
        : parameters->range_of_values_per_component_lan;

    out->x = map_to_fixed_width(in->n[0], parameters->min_value, parameters->max_value, range);
    out->y = map_to_fixed_width(in->n[1], parameters->min_value, parameters->max_value, range);
    out->z = map_to_fixed_width(in->n[2], parameters->min_value, parameters->max_value, range);
}
