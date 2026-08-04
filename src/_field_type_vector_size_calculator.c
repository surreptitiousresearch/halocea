/* _field_type_vector_size_calculator — message-delta "vector" field: 32 bits per component
 * (protocol-local wire width) times the component count in parameters[0], plus overhead_bits. */

#include "headers/field_properties_definition.h"

int _field_type_vector_size_calculator(_field_properties_definition *const field_properties_definition)
{
    int *parameters = (int *)field_properties_definition->parameters;
    int v = *parameters;
    field_properties_definition->overhead_bits = v;
    return 32 * v + v;
}
