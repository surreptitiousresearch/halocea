/* _field_type_integer_parameter_verifier @0x8379A768 */
/* _field_type_integer_parameter_verifier — the parameters blob leads with a _field_width, but
 * the shipped bounds check compares it against the field_type count (_field_type_LAST_POSITION == 28,
 * DB enum _field_type). This is a loose sanity bound reused from the generic field-type verifier. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/_field_type.h"

uint8_t _field_type_integer_parameter_verifier(const _field_properties_definition *const field_properties_definition)
{
    int v = *(int *)field_properties_definition->parameters;
    if (v < 0)
        return 0;
    if (v >= _field_type_LAST_POSITION)
        return 0;
    return 1;
}
