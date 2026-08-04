/* _field_type_fixed_width_normal_vector_parameter_verifier @0x8379C658 — field_parameters_verifier for the
 * "fixed width normal vector" field type. While the protocol changeover config system is active, registers
 * the theta/phi bit-width parameters (parameters[0]/[1]) as tunable integers. Valid only if all 4 parameter
 * slots (theta/phi bit widths plus 2 more) are positive. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/parameter_type.h"
#include "headers/blam_data_globals.h"

extern void message_delta_parameters_protocol_expose_parameter(const char *type_name, const char *parameter_name, parameter_type type, void *location);

uint8_t _field_type_fixed_width_normal_vector_parameter_verifier(const _field_properties_definition *const field_properties_definition)
{
    int *parameters = (int *)field_properties_definition->parameters;

    if ( g_protocol_changeover_enabled == 1 )
    {
        const char *name = field_properties_definition->name;
        message_delta_parameters_protocol_expose_parameter(field_properties_definition->name, "bits_theta_internet", INTEGER, &parameters[0]);
        message_delta_parameters_protocol_expose_parameter(name, "bits_phi_internet", INTEGER, &parameters[1]);
    }

    if ( parameters[0] <= 0 )
        return 0;
    if ( parameters[1] <= 0 )
        return 0;
    if ( parameters[2] <= 0 )
        return 0;
    if ( parameters[3] <= 0 )
        return 0;

    return 1;
}
