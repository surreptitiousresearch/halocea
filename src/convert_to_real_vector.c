/* convert_to_real_vector @0x8379D5D0 — unquantize a 3-component vector back from a smart_vector field
 * type's fixed-width integer encoding into floats, using the LAN range (range_of_values_per_component_lan)
 * unless gMessageEncodingClass is non-zero, in which case the narrower internet range
 * (range_of_values_per_component_internet) is used for all three components. Exact mathematical inverse of
 * the already-done sibling convert_to_quantized_smart.c/map_to_fixed_width.
 *
 * DEVIATION: Hex-Rays flagged "local variable allocation has failed" and fabricated a nonexistent 4th
 * parameter (`a4`) plus `__int64`/HIDWORD-pun garbage throughout; the DB's own prototype confirms only 3
 * real parameters (in, out, parameters — the 3rd a pointer, not the `__int64` the raw decompile showed).
 * disasm_range(0x8379D5D0,0x8379D760) resolves the actual arithmetic cleanly: per component, the classic
 * "store-then-load" int-to-float conversion idiom (`std`/`lfd`/`fcfid`/`frsp`) is just `(float)in->x`; the
 * quantization range denominator (parameters+0xC or +0x14, selected once by gMessageEncodingClass exactly
 * as in convert_to_quantized_smart.c) and the min/max span (`max_value - min_value`, computed once up front
 * and reused for all 3 components) combine as `(float)component / (float)range * value_range + min_value`. */

#include "headers/real_vector3d.h"
#include "headers/field_type_smart_vector_parameters.h"

#include "headers/message_delta_encoding_class.h"
#include "headers/blam_data_globals.h"

void convert_to_real_vector(const quantized_smart_vector *const in, real_vector3d *const out,
    const _field_type_smart_vector_parameters *const parameters)
{
    unsigned int range = gMessageEncodingClass
        ? parameters->range_of_values_per_component_internet
        : parameters->range_of_values_per_component_lan;

    float value_range = parameters->max_value - parameters->min_value;

    out->n[0] = (float)in->x / (float)range * value_range + parameters->min_value;
    out->n[1] = (float)in->y / (float)range * value_range + parameters->min_value;
    out->n[2] = (float)in->z / (float)range * value_range + parameters->min_value;
}
