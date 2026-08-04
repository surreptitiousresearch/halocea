/* _field_type_structure_size_calculator @0x8379B250 — maximum_size_calculator for the "structure" field
 * type: sums each member's own maximum_size_calculator result, writes each member's computed maximum_size
 * back into its properties, and records the member count as this field's overhead_bits.
 *
 * DEVIATION: the decompiler rendered the per-member callback call as zero-arg; disasm_range(0x8379B250,
 * 0x8379B2D4) shows r3 (the member's _field_properties_definition*, loaded from parameters[1] of each
 * stride) is untouched before mtctr/bctrl — same bctr-tail-call arg-loss class as
 * _field_type_array_size_calculator.c. The rest of the loop's raw pointer-stride walk is reproduced as
 * written since it matches the disasm exactly. */

#include "headers/field_properties_definition.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


int _field_type_structure_size_calculator(_field_properties_definition *const field_properties_definition)
{
    int *parameters = (int *)field_properties_definition->parameters;
    int total_size = 0;
    int member_count = 0;
    int count = *parameters;

    if ( *parameters > 0 )
    {
        int *member = parameters - 3;
        do
        {
            _field_properties_definition *member_properties = (_field_properties_definition *)member[4];
            int member_size = message_delta_global_field_type_list[member_properties->type].maximum_size_calculator(member_properties);

            member += 4;
            ++member_count;
            total_size += member_size;
            member_properties->maximum_size = member_size; /* was *(int*)(*member+92); +92 == _field_properties_definition.maximum_size (DB) */
            count = *parameters;
        }
        while ( member_count < *parameters );
    }

    field_properties_definition->overhead_bits = count;
    return count + total_size;
}
