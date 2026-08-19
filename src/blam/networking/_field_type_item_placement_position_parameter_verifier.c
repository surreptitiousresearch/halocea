/* _field_type_item_placement_position_parameter_verifier @0x8379DD68 — field_parameters_verifier for the
 * "item placement position" field type. Always valid; while the protocol changeover config system is
 * active, additionally registers the gITEM_PLACEMENT_BITS_X/Y/Z globals as tunable integer parameters. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/parameter_type.h"
#include "headers/blam_data_globals.h"

extern void message_delta_parameters_protocol_expose_parameter(const char *type_name, const char *parameter_name, parameter_type type, void *location);

uint8_t _field_type_item_placement_position_parameter_verifier(const _field_properties_definition *const field_properties_definition)
{
    if ( g_protocol_changeover_enabled == 1 )
    {
        message_delta_parameters_protocol_expose_parameter(0, "gITEM_PLACEMENT_BITS_X", INTEGER, &gITEM_PLACEMENT_BITS_X);
        message_delta_parameters_protocol_expose_parameter(0, "gITEM_PLACEMENT_BITS_Y", INTEGER, &gITEM_PLACEMENT_BITS_Y);
        message_delta_parameters_protocol_expose_parameter(0, "gITEM_PLACEMENT_BITS_Z", INTEGER, &gITEM_PLACEMENT_BITS_Z);
    }

    return 1;
}
