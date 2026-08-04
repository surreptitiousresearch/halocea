/* field_translated_index_unregister_local_index @0x8379BCD4 — for an index-translated network field,
 * release the mapping for one local index: look up the server index it resolves to, clear the
 * back-reference slot, and unset the resolution entry. Returns 1 if a mapping was removed, else 0.
 *
 * The parameters blob holds the index_resolution_table at +12 and the local-index back-reference
 * array pointer at dword +10 (+40 bytes). */

#include "headers/field_properties_definition.h"
#include "headers/_field_type_translated_index_parameters.h"

#include "headers/index_resolution_table.h"
extern int index_resolution_get(const index_resolution_table *table, int server_object_index);
extern void index_resolution_set(index_resolution_table *table, int server_object_index, int local_object_index);

int field_translated_index_unregister_local_index(const _field_properties_definition *field_properties_definition, int local_index)
{
    _field_type_translated_index_parameters *parameters =
        (_field_type_translated_index_parameters *)field_properties_definition->parameters;
    /* recovered: (index_resolution_table *)(parameters + 12) -> &parameters->translation_table (offset 0x0C) */
    int server_index = index_resolution_get(&parameters->translation_table, local_index);
    if ( server_index == -1 )
        return 0;
    /* recovered: *(int *)(*((int *)parameters + 10) + 4 * server_index) -> translated_index_allocations[server_index] (offset 0x28) */
    parameters->translated_index_allocations[server_index] = -1;
    index_resolution_set(&parameters->translation_table, local_index, -1);
    return 1;
}
