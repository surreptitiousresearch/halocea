/* field_properties_initialize @0x837A11B8 — one-time lazy init of a field-properties definition: verifies
 * its parameters and caches its maximum serialized size. Mirrors field_properties_verify_parameters.c's fix
 * for the decompiler's "renders a real tail-call as a zero-arg call" artifact. */

#include "headers/field_properties_definition.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


void field_properties_initialize(_field_properties_definition *const field_properties_definition)
{
    if ( !field_properties_definition->initialized )
    {
        message_delta_global_field_type_list[field_properties_definition->type].field_parameters_verifier(
            field_properties_definition);
        field_properties_definition->maximum_size =
            message_delta_global_field_type_list[field_properties_definition->type].maximum_size_calculator(
                field_properties_definition);
        field_properties_definition->initialized = 1;
    }
}
