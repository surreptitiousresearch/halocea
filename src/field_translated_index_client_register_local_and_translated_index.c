/* field_translated_index_client_register_local_and_translated_index @0x8379BB88 — client-side: register an
 * explicit (local_index, translated_index) pair for an index-translated field, failing if the translated
 * slot is already occupied or the local index is already registered. Parameters-blob layout (capacity@0,
 * cursor@36, slots@40, index_resolution_table@12) established by
 * field_translated_index_server_register_local_index.c. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/_field_type_translated_index_parameters.h"

#include "headers/index_resolution_table.h"
#include "headers/index_resolution_table.h"
extern int index_resolution_get(const index_resolution_table *table, int server_object_index);
extern void index_resolution_set(index_resolution_table *table, int server_object_index, int local_object_index);

uint8_t field_translated_index_client_register_local_and_translated_index(const _field_properties_definition *const field_properties_definition, const int local_index, const int translated_index)
{
    _field_type_translated_index_parameters *parameters =
        (_field_type_translated_index_parameters *)field_properties_definition->parameters;
    /* recovered: (int *)*((int *)parameters + 10) -> translated_index_allocations (offset 0x28) */
    int *slots = parameters->translated_index_allocations;

    /* recovered: (index_resolution_table *)(parameters + 12) -> &parameters->translation_table (offset 0x0C) */
    if ( slots[translated_index] != -1
      || index_resolution_get(&parameters->translation_table, local_index) != -1 )
    {
        return 0;
    }

    slots[translated_index] = local_index;
    index_resolution_set(&parameters->translation_table, local_index, translated_index);
    return 1;
}
