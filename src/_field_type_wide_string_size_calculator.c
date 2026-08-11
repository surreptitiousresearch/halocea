/* message-delta "wide_string" field: 16 bits per wide character (protocol-local wire width)
 * times the max length in parameters[0], plus the overhead bits needed to encode the length. */

#include "headers/field_properties_definition.h"

extern int bits_needed_for_value(int value);

int _field_type_wide_string_size_calculator(_field_properties_definition *const field_properties_definition)
{
    int *parameters = (int *)field_properties_definition->parameters;
    int overhead_bits = bits_needed_for_value(*parameters);

    field_properties_definition->overhead_bits = overhead_bits;
    return 16 * *parameters + overhead_bits;
}
