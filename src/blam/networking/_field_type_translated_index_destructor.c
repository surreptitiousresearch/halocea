/* _field_type_translated_index_destructor @0x8379B9E0 — free the translated-index field type's parameter
 * allocations: dlFree the translated_index_allocations block, then shut down the embedded translation table.
 *
 * DEVIATION: the previous reconstruction was an empty no-op; the disassembly (0x8379B9F0-0x8379BA18) shows
 * two dropped calls (dlFree, index_resolution_shutdown). Restored. Original-binary quirk preserved: the
 * parameters pointer starts NULL and is only loaded when
 * message_delta_global_field_type_list[_field_type_translated_index].requires_parameters == 1, yet both
 * teardown calls run unconditionally — with requires_parameters != 1 this would dereference NULL (in
 * practice the list entry always requires parameters). */

#include "headers/field_properties_definition.h"
#include "headers/field_type_definition.h"
#include "headers/_field_type.h"
#include "headers/_field_type_translated_index_parameters.h"
#include "headers/blam_data_globals.h"

extern void dlFree(void *ptr);
extern void index_resolution_shutdown(index_resolution_table *table);

void _field_type_translated_index_destructor(const _field_properties_definition *const field_properties_definition)
{
    _field_type_translated_index_parameters *parameters = 0;
    if ( message_delta_global_field_type_list[_field_type_translated_index].requires_parameters == 1 )
        parameters = (_field_type_translated_index_parameters *)field_properties_definition->parameters;
    dlFree(parameters->translated_index_allocations);
    index_resolution_shutdown(&parameters->translation_table);
}
