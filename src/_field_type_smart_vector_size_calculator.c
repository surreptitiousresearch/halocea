/* _field_type_smart_vector_size_calculator @0x8379D840 — the "smart vector" field type's
 * maximum_size_calculator callback: worst-case bit budget = common-vector selector bits plus the larger of
 * the internet and LAN per-component budgets (3 components each), plus one bit for the common-vector escape
 * flag. */

#include "headers/field_properties_definition.h"
#include "headers/field_type_smart_vector_parameters.h"

int _field_type_smart_vector_size_calculator(_field_properties_definition *const field_properties_definition)
{
    const _field_type_smart_vector_parameters *parameters =
            (const _field_type_smart_vector_parameters *)field_properties_definition->parameters;

    int selector_bits = parameters->number_of_common_vectors;
    int internet_bits = 3 * parameters->number_of_bits_per_component_internet;
    int lan_bits = 3 * parameters->number_of_bits_per_component_lan;

    int larger;
    if (selector_bits + lan_bits + 1 <= selector_bits + internet_bits + 1)
        larger = selector_bits + internet_bits;
    else
        larger = selector_bits + lan_bits;
    return larger + 1;
}
