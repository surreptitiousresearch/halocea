/* _field_type_fixed_width_size_calculator @0x8379C3C0 — reads the first int of the field's parameters blob
 * (confirmed via disasm: `lwz r11, 0x58(r3); lwz r3, 0(r11)`, i.e. `*(int *)field_properties_definition->
 * parameters`). For fixed-width fields the parameters blob begins with the fixed bit width. Identical body
 * to _field_type_flags_size_calculator.c; both read the same leading-int convention from their own field's
 * parameters blob. */

#include "headers/field_properties_definition.h"

int _field_type_fixed_width_size_calculator(_field_properties_definition *const field_properties_definition)
{
    return *(int *)field_properties_definition->parameters;
}
