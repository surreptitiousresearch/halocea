/* _field_type_flags_size_calculator @0x8379C0A8 — reads the first int of the field's parameters blob
 * (confirmed via disasm: `lwz r11, 0x58(r3); lwz r3, 0(r11)`, i.e. `*(int *)field_properties_definition->
 * parameters`). For flags fields the parameters blob begins with the flag count. */

#include "headers/field_properties_definition.h"

int _field_type_flags_size_calculator(_field_properties_definition *const field_properties_definition)
{
    return *(int *)field_properties_definition->parameters;
}
