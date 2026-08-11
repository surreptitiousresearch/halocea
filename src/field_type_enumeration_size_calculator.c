/* _field_type_enumeration_size_calculator @0x8379B798 — the enumeration field type's maximum_size_calculator
 * callback: byte size selected by the parameters' `width` (0=1 byte, 1=2 bytes, else 4 bytes). */

#include "headers/field_properties_definition.h"
#include "headers/field_type_enumeration_parameters.h"

int _field_type_enumeration_size_calculator(_field_properties_definition *const field_properties_definition)
{
    _field_width width = ((const _field_type_enumeration_parameters *)field_properties_definition->parameters)->width;
    if (!width)
        return 1;
    if (width == 1)
        return 2;
    return 4;
}
