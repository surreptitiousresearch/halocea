/* field_translated_index_translate_index @0x8379BC00 — translate a local object index into the network/
 * global index used on the wire, via the field-properties definition's index_resolution_table (located at
 * +12 within the definition's parameters blob). A local index of -1, or one with no resolved entry, maps to
 * 0. */

#include "headers/field_properties_definition.h"
#include "headers/_field_type_translated_index_parameters.h"

#include "headers/index_resolution_table.h"
extern int index_resolution_get(const index_resolution_table *table, int server_object_index);

int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition,
                                           int local_index)
{
    if ( local_index == -1 )
        return 0;

    _field_type_translated_index_parameters *parameters =
        (_field_type_translated_index_parameters *)field_properties_definition->parameters;
    /* recovered: (index_resolution_table *)(parameters + 12) -> &parameters->translation_table (offset 0x0C) */
    int translated = index_resolution_get(&parameters->translation_table, local_index);
    return translated == -1 ? 0 : translated;
}
