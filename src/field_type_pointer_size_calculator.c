/* field_type_pointer_size_calculator @0x8379B660 — maximum_size_calculator for the "pointer" field type:
 * `parameters` holds a pointer to the pointed-to field's own _field_properties_definition. Recurses into
 * that inner definition's type-specific size calculator and caches the result back into the inner
 * definition's `maximum_size`.
 *
 * DEVIATION: the decompiler rendered the tail-call callback as zero-arg (`(int(*)(void))...()`), but
 * disasm_range(0x8379B660,0x8379B6B0) shows `lwz r3, 0(r31)` loading the inner definition pointer into r3
 * immediately before `bctrl` — the callback is called with that inner definition as its argument, matching
 * field_type_definition.h's `maximum_size_calculator(_field_properties_definition *)` signature. */

#include "headers/field_properties_definition.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


int field_type_pointer_size_calculator(_field_properties_definition *const field_properties_definition)
{
    _field_properties_definition *inner = *(_field_properties_definition **)field_properties_definition->parameters;
    int result = message_delta_global_field_type_list[inner->type].maximum_size_calculator(inner);

    inner->maximum_size = result;
    return result;
}
