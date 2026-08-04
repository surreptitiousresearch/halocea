/* field_translated_index_translate_index_no_default @0x8379BC4C — like
 * field_translated_index_translate_index, but returns the index_resolution_table's raw result (which may be
 * -1) instead of defaulting an unresolved index to 0. A local index of -1 maps to 0. */

#include "headers/field_properties_definition.h"
#include "headers/_field_type_translated_index_parameters.h"

#include "headers/index_resolution_table.h"
extern int index_resolution_get(const index_resolution_table *table, int server_object_index);

int field_translated_index_translate_index_no_default(
        const _field_properties_definition *const field_properties_definition, int local_index)
{
    if ( local_index == -1 )
        return 0;
    /* attested: void* -> struct* is implicit in C; redundant cast removed */
    _field_type_translated_index_parameters *parameters = field_properties_definition->parameters;
    /* recovered: (index_resolution_table *)(parameters + 12) -> &parameters->translation_table (offset 0x0C) */
    return index_resolution_get(&parameters->translation_table, local_index);
}
