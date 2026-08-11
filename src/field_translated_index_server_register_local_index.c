/* field_translated_index_server_register_local_index @0x8379BAD0 — server-side: assigns a translated
 * (network) index to a local index for an index-translated field, if it doesn't already have one. It
 * scans the translation slot array as a ring starting at the rolling cursor for the first free slot
 * (value -1), records the mapping in both the index_resolution_table and the slot array, and advances
 * the cursor. Returns the translated index, or -1 if the table is full. */

#include "headers/field_properties_definition.h"
#include "headers/_field_type_translated_index_parameters.h"

#include "headers/index_resolution_table.h"
#include "headers/index_resolution_table.h"
extern int index_resolution_get(const index_resolution_table *table, int server_object_index);
extern void index_resolution_set(index_resolution_table *table, int server_object_index, int local_object_index);

int field_translated_index_server_register_local_index(
        const _field_properties_definition *field_properties_definition, int local_index)
{
    _field_type_translated_index_parameters *parameters =
        (_field_type_translated_index_parameters *)field_properties_definition->parameters;
    /* recovered: (index_resolution_table *)(parameters + 12) -> &parameters->translation_table (offset 0x0C) */
    int translated_index = index_resolution_get(&parameters->translation_table, local_index);
    if (translated_index == -1)
    {
        /* recovered: *((int *)parameters + 9) -> translated_index_allocation_cursor (offset 0x24);
         * *(int *)parameters -> maximum_active_at_once (offset 0x00);
         * *((int *)parameters + 10) -> translated_index_allocations (offset 0x28) */
        int cursor = parameters->translated_index_allocation_cursor;
        int capacity = parameters->maximum_active_at_once;
        int *slots = parameters->translated_index_allocations;
        int start = cursor;
        while (1)
        {
            if (slots[cursor] == -1)
                translated_index = cursor;
            int next = cursor + 1;
            parameters->translated_index_allocation_cursor = next;
            if (next >= capacity)
                parameters->translated_index_allocation_cursor = 0;
            cursor = parameters->translated_index_allocation_cursor;
            if (start == cursor)
                break;
            if (translated_index != -1)
                break;
        }
        if (translated_index == -1)
            return translated_index;
        index_resolution_set(&parameters->translation_table, local_index, translated_index);
        parameters->translated_index_allocations[translated_index] = local_index;
    }
    return translated_index;
}
