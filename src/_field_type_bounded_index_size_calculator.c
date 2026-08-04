#include "headers/field_properties_definition.h"

extern int bits_needed_for_value(int value);

int field_type_bounded_index_size_calculator(
        _field_properties_definition *const field_properties_definition)
{
    return bits_needed_for_value(
        ((unsigned int *)field_properties_definition->parameters)[1] -
        ((unsigned int *)field_properties_definition->parameters)[0]);
}
