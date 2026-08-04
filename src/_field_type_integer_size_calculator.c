/* _field_type_integer_size_calculator — maps the field's _field_width parameter to its
 * serialized bit width. DB-verified enum _field_width (in field_type_enumeration_parameters.h). */

#include "headers/field_properties_definition.h"
#include "headers/field_type_enumeration_parameters.h"

/* DEVIATION: the pointee is non-const — the maximum_size_calculator slot is
 * `int (*)(_field_properties_definition *const)` (DB types_members +0x08) and the DB prototype
 * agrees. Every sibling calculator in message_delta_global_field_type_list[28] already matched;
 * this one had drifted to a const pointee. */
int _field_type_integer_size_calculator(_field_properties_definition *const field_properties_definition)
{
    const _field_width *width = (const _field_width *)field_properties_definition->parameters;

    switch ( *width )
    {
        case _field_width_small:  return 8;
        case _field_width_medium: return 16;
        case _field_width_large:  return 32;
        case _field_width_1_bit:  return 1;
        case _field_width_3_bits: return 3;
        case _field_width_5_bits: return 5;
        default:                  return 6; /* _field_width_6_bits */
    }
}
