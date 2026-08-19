/* _field_type_array_size_calculator @0x8379AE88 — maximum_size_calculator for the "array" field type.
 * DEVIATION: same bctr-tail-call arg-loss class as field_type_pointer_size_calculator.c — disasm_range
 * (0x8379AE88,0x8379AEE0) shows `lwz r3, 8(r29)` (parameters[2], the element's own
 * _field_properties_definition*) loaded into r3 immediately before `bctrl`, so the callback is called with
 * that inner definition as its argument, not zero-arg. */

#include "headers/field_properties_definition.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


int _field_type_array_size_calculator(_field_properties_definition *const field_properties_definition)
{
    int *parameters = (int *)field_properties_definition->parameters;
    int element_count = parameters[0];
    _field_properties_definition *element_definition = (_field_properties_definition *)parameters[2];
    int element_size = message_delta_global_field_type_list[element_definition->type].maximum_size_calculator(element_definition);

    element_definition->maximum_size = element_size;
    field_properties_definition->overhead_bits = element_count;
    return element_count * element_size + element_count;
}
