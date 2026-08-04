/* field_properties_calculate_maximum_size @0x837A0B08 — dispatches to the field type's
 * maximum_size_calculator callback (real tail call — disasm 0x837A0B08-0x837A0B2C ends in `bctr`, so
 * field_properties_definition, already in r3, is passed through unchanged; the decompiler mis-rendered this
 * as a zero-arg call). */

#include "headers/field_properties_definition.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


int field_properties_calculate_maximum_size(_field_properties_definition *const field_properties_definition)
{
    return message_delta_global_field_type_list[field_properties_definition->type].maximum_size_calculator(field_properties_definition);
}
