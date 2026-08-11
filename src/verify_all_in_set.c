/* verify_all_in_set @0x837A1068 — verifies every field in a message field-reference set by dispatching to
 * each field's type-specific field_parameters_verifier (see field_properties_verify_parameters.c — the
 * decompiler renders this dispatch as a zero-arg call, but disasm there confirms it's a real tail call
 * passing the field's properties through unchanged; applied identically here). Stops early at a zeroed
 * (sentinel/unused) field reference. */

#include "headers/message_definition_field_reference_set.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


void verify_all_in_set(_message_definition_field_reference_set *const field_reference_set)
{
    int index = 0;

    if ( field_reference_set->count > 0 )
    {
        _field_reference_definition *field_reference = field_reference_set->field_references;
        do
        {
            if ( !field_reference->offset && !field_reference->baseline_offset && !field_reference->properties )
                break;

            message_delta_global_field_type_list[field_reference->properties->type].field_parameters_verifier(
                field_reference->properties);

            ++index;
            ++field_reference;
        }
        while ( index < field_reference_set->count );
    }
}
