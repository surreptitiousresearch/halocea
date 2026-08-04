#include <stdint.h>
#include "headers/field_properties_definition.h"

uint8_t _field_type_bounded_index_parameter_verifier(const _field_properties_definition *const field_properties_definition)
{
    int *params = (int *)field_properties_definition->parameters;
    return (unsigned char)(((unsigned int)((params[1] ^ params[0]) >= 0) + (unsigned int)(params[0] >= params[1])) & 1u);
}
