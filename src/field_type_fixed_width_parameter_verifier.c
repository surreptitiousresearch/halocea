/* field_type_fixed_width_parameter_verifier @0x8379C3D0 — the fixed-width field type's
 * field_parameters_verifier callback: valid only when both number_of_bits and range_of_values are positive. */

#include "headers/field_properties_definition.h"
#include "headers/field_type_fixed_width_parameters.h"

unsigned __int8 field_type_fixed_width_parameter_verifier(const _field_properties_definition *const field_properties_definition)
{
    const _field_type_fixed_width_parameters *parameters = (const _field_type_fixed_width_parameters *)field_properties_definition->parameters;
    return parameters->number_of_bits > 0 && parameters->range_of_values > 0;
}
