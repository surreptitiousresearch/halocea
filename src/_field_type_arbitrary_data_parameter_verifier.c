#include "headers/field_properties_definition.h"

unsigned int _field_type_arbitrary_data_parameter_verifier(
        const _field_properties_definition *const field_properties_definition)
{
    unsigned int n = *(const unsigned int *)field_properties_definition->parameters;
    return (-n & ~n) >> 31;
}
