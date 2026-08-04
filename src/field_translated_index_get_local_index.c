#include "headers/field_properties_definition.h"
#include "headers/_field_type_translated_index_parameters.h"

int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index)
{
    if (!translated_index)
        return -1;
    _field_type_translated_index_parameters *parameters =
        (_field_type_translated_index_parameters *)field_properties_definition->parameters;
    /* recovered: *((int *)parameters + 10) -> translated_index_allocations (offset 0x28), the
     * translated->local index back-reference array, indexed by translated_index. */
    return parameters->translated_index_allocations[translated_index];
}
