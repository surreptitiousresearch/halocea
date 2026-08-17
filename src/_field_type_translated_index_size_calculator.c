/* _field_type_translated_index_size_calculator @0x8379B8E8 */
/* _field_type_translated_index_size_calculator — maximum_size_calculator for the "translated index"
 * field type: serialized width is the number of bits needed to represent maximum_active_at_once,
 * cached back into the parameters' number_of_bits field. */

#include "headers/field_properties_definition.h"
#include "headers/_field_type_translated_index_parameters.h"

extern int bits_needed_for_value(int value);

int _field_type_translated_index_size_calculator(_field_properties_definition *const field_properties_definition)
{
    _field_type_translated_index_parameters *parameters =
        (_field_type_translated_index_parameters *)field_properties_definition->parameters;
    int size = bits_needed_for_value(parameters->maximum_active_at_once);
    parameters->number_of_bits = size; /* was parameters[2]; +8 == number_of_bits (DB) */
    return size;
}
