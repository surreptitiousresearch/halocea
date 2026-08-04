/* field_properties_verify_parameters @0x837A0AE0 — dispatches to the field type's field_parameters_verifier
 * callback (real tail call — disasm 0x837A0AE0-0x837A0B04 ends in `bctr`, so field_properties_definition,
 * already in r3, is passed through unchanged; the decompiler mis-rendered this as a zero-arg call). */

#include "headers/field_properties_definition.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


unsigned __int8 field_properties_verify_parameters(const _field_properties_definition *const field_properties_definition)
{
    return message_delta_global_field_type_list[field_properties_definition->type].field_parameters_verifier(field_properties_definition);
}
