#include "headers/field_properties_definition.h"

int _field_type_point_size_calculator(_field_properties_definition *const field_properties_definition)
{
    int *parameters = (int *)field_properties_definition->parameters;
    int v = *parameters;
    field_properties_definition->overhead_bits = v;
    return 32 * v + v;
}
