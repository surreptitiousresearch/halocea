/* _field_type_array_parameter_verifier @0x8379AEF0 — field_parameters_verifier for the "array" field type:
 * validates the element count, element size, and element-definition parameters, then defers to the
 * element type's own verifier.
 *
 * DEVIATION: the decompiler rendered the nested verifier call as zero-arg; disasm_range(0x8379AEF0,
 * 0x8379AF74) shows r3 (the element _field_properties_definition*, loaded from parameters[2]) is never
 * reassigned before mtctr/bctrl — same bctr-tail-call arg-loss class as _field_type_array_size_calculator.c
 * and field_type_pointer_parameter_verifier.c. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


uint8_t _field_type_array_parameter_verifier(const _field_properties_definition *const field_properties_definition)
{
    int *parameters = (int *)field_properties_definition->parameters;

    if ( parameters[0] <= 0 )
        return 0;
    if ( parameters[1] <= 0 )
        return 0;

    _field_properties_definition *element_definition = (_field_properties_definition *)parameters[2];
    if ( !element_definition )
        return 0;

    return message_delta_global_field_type_list[element_definition->type].field_parameters_verifier(element_definition) == 1;
}
