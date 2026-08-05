/* _field_type_flags_parameter_verifier @0x8379C0B8 — validates a flags field's parameters blob: the
 * leading int is a bit count in [1,32], and each following byte (one per bit) must be 0 or 1. */

#include <stdint.h>
#include "headers/field_properties_definition.h"

/* DEVIATION: returns uint8_t, not int — the _field_type_definition.field_parameters_verifier slot
 * is `unsigned __int8 (*)(const _field_properties_definition *const)` (DB types_members +0x0C), the
 * DB prototype agrees, and the return traces to a byte-wide 0/1. Every sibling verifier in
 * message_delta_global_field_type_list[28] already carried uint8_t; this one had drifted to int. */
uint8_t _field_type_flags_parameter_verifier(const _field_properties_definition *const field_properties_definition)
{
    const int *parameters = (const int *)field_properties_definition->parameters;
    int bit_count = parameters[0];

    if (bit_count > 0 && bit_count <= 32)
    {
        const uint8_t *flags = (const uint8_t *)(parameters + 1);
        for (int i = 0; i < bit_count; i++)
        {
            if (flags[i] != 0 && flags[i] != 1)
                return 0;
        }
    }
    else
    {
        return 0;
    }

    return 1;
}
