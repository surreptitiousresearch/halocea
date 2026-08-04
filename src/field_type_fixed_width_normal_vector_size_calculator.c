/* field_type_fixed_width_normal_vector_size_calculator @0x8379C610 — the fixed-width normal-vector field
 * type's maximum_size_calculator callback: the larger of the internet (theta+phi) and LAN (theta+phi) bit
 * budgets. */

#include "headers/field_properties_definition.h"
#include "headers/field_type_fixed_width_normal_vector_parameters.h"

int field_type_fixed_width_normal_vector_size_calculator(_field_properties_definition *const field_properties_definition)
{
    const _field_type_fixed_width_normal_vector_parameters *parameters =
            (const _field_type_fixed_width_normal_vector_parameters *)field_properties_definition->parameters;

    int internet_bits = parameters->number_of_bits_theta_internet + parameters->number_of_bits_phi_internet;
    int lan_bits = parameters->number_of_bits_theta_lan + parameters->number_of_bits_phi_lan;

    if (lan_bits > internet_bits)
        return lan_bits;
    return internet_bits;
}
