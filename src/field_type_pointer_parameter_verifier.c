/* _field_type_pointer_parameter_verifier @0x8379B6C0 — field_parameters_verifier for the "pointer" field
 * type: `parameters` holds a pointer to the pointed-to field's own _field_properties_definition, whose
 * type-specific verifier is consulted.
 *
 * DEVIATION: the decompiler rendered the callback as zero-arg (`(...)()`), but disasm_range(0x8379B6C0,
 * 0x8379B710) shows r3 — loaded with the inner definition pointer at 0x8379B6D0 — is never reassigned
 * before the mtctr/bctrl sequence, so the callback is called with that inner definition as its argument,
 * matching field_type_definition.h's field_parameters_verifier(const _field_properties_definition *)
 * signature (same shape as field_type_pointer_size_calculator.c). */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


uint8_t _field_type_pointer_parameter_verifier(const _field_properties_definition *const field_properties_definition)
{
    const _field_properties_definition *inner = *(const _field_properties_definition **)field_properties_definition->parameters;

    return inner && message_delta_global_field_type_list[inner->type].field_parameters_verifier(inner) == 1;
}
