/* _field_type_locality_reference_position_parameter_verifier @0x8379CBC0 — field_parameters_verifier for the
 * "locality reference position" field type. Always valid; while the protocol changeover config system is
 * active, additionally registers the gLOCALITY_* globals as tunable parameters. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/parameter_type.h"
#include "headers/blam_data_globals.h"

extern void message_delta_parameters_protocol_expose_parameter(const char *type_name, const char *parameter_name, parameter_type type, void *location);

uint8_t _field_type_locality_reference_position_parameter_verifier(const _field_properties_definition *const field_properties_definition)
{
    if ( g_protocol_changeover_enabled == 1 )
    {
        message_delta_parameters_protocol_expose_parameter(0, "LOCALITY_BITS_PER_COMPONENT_FULL", INTEGER,
            &gLOCALITY_BITS_PER_COMPONENT_FULL_INTERNET);
        message_delta_parameters_protocol_expose_parameter(0, "LOCALITY_BITS_PER_COMPONENT_DELTA", INTEGER,
            &gLOCALITY_BITS_PER_COMPONENT_DELTA_INTERNET);
        message_delta_parameters_protocol_expose_parameter(0, "LOCALITY_DELTA_CUTOFF_DISTANCE", REAL,
            &gLOCALITY_DELTA_CUTOFF_DISTANCE_INTERNET);
        message_delta_parameters_protocol_expose_parameter(0, "LOCALITY_MINIMUM_MOVE_DISTANCE", REAL,
            &gLOCALITY_MINIMUM_MOVE_DISTANCE_INTERNET);
    }

    return 1;
}
