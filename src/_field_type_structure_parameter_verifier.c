/* _field_type_structure_parameter_verifier @0x8379B2D8 — field_parameters_verifier for the "structure"
 * field type: parameters is a count followed inline by that many _field_reference_definition entries;
 * every entry's properties pointer must be non-null and pass its own type's field_parameters_verifier.
 *
 * DEVIATION: the decompiler rendered the nested verifier call as zero-arg; disasm (0x8379B2E8-0x8379B398)
 * shows r3 (the element _field_properties_definition*, already loaded into r3 right before mtctr/bctrl) is
 * passed through unchanged — same bctr-tail-call arg-loss class as _field_type_array_parameter_verifier.c.
 * The decompiler also completely dropped a leading dead computation: the compiled code reads
 * message_delta_global_field_type_list[9].requires_parameters into a scratch register before checking the
 * count, but every path that would have used that value re-assigns the return register to a literal 0
 * first, so it's never observable — omitted here as genuinely dead code, not a logic difference. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/field_type_definition.h"
#include "headers/field_reference_definition.h"
#include "headers/blam_data_globals.h"


uint8_t _field_type_structure_parameter_verifier(const _field_properties_definition *const field_properties_definition)
{
    int *parameters = (int *)field_properties_definition->parameters;
    int count = *parameters;
    if ( count <= 0 )
        return 0;

    const _field_reference_definition *field_reference = (const _field_reference_definition *)(parameters + 1);
    for ( int i = 0; i < count; ++i, ++field_reference )
    {
        _field_properties_definition *properties = field_reference->properties;
        if ( !properties )
            return 0;
        if ( message_delta_global_field_type_list[properties->type].field_parameters_verifier(properties) != 1 )
            return 0;
    }

    return 1;
}
