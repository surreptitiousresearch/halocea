#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/_field_type_translated_index_parameters.h"

#include "headers/index_resolution_table.h"
extern int index_resolution_get(const index_resolution_table *table, int server_object_index);

uint8_t field_translated_index_is_local_index_registered(const _field_properties_definition *const field_properties_definition, const int local_index)
{
    _field_type_translated_index_parameters *parameters =
        (_field_type_translated_index_parameters *)field_properties_definition->parameters;
    /* recovered: (index_resolution_table *)(parameters + 12) -> &parameters->translation_table (offset 0x0C) */
    return index_resolution_get(&parameters->translation_table, local_index) != -1;
}
