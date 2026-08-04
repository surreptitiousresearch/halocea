/* field_type_dispose @0x837A0A48 — clears a field type definition's initialized flag (offset 0x14 =
 * _field_type_definition::initialized). */

#include "headers/field_type_definition.h"

void field_type_dispose(_field_type_definition *const field_type_definition)
{
    field_type_definition->initialized = 0;
}
