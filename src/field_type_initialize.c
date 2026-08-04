/* field_type_initialize @0x837A0A38 — marks a field type definition initialized (offset 0x14 =
 * _field_type_definition::initialized). */

#include "headers/field_type_definition.h"

void field_type_initialize(_field_type_definition *const field_type_definition)
{
    field_type_definition->initialized = 1;
}
