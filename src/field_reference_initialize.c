/* field_reference_initialize @0x837A1E8C — one-time lazy init of a field reference: initializes its
 * properties definition inline (same verify + cache-max-size steps as field_properties_initialize.c) before
 * marking the reference itself initialized. */

#include "headers/field_reference_definition.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


void field_reference_initialize(_field_reference_definition *const field_reference_definition)
{
    if ( !field_reference_definition->initialized )
    {
        _field_properties_definition *properties = field_reference_definition->properties;

        if ( !properties->initialized )
        {
            message_delta_global_field_type_list[properties->type].field_parameters_verifier(properties);
            properties->maximum_size =
                message_delta_global_field_type_list[properties->type].maximum_size_calculator(properties);
            properties->initialized = 1;
        }

        field_reference_definition->initialized = 1;
    }
}
