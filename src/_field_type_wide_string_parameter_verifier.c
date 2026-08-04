/* message-delta "wide_string" field parameter verifier: returns 1 iff the length in
 * parameters[0] is a power of two ((-n & ~n) >> 31); protocol-local validation. */

#include "headers/field_properties_definition.h"

unsigned int field_type_wide_string_parameter_verifier(
        const _field_properties_definition *const field_properties_definition)
{
    unsigned int n = *(const unsigned int *)field_properties_definition->parameters;
    return (-n & ~n) >> 31;
}
