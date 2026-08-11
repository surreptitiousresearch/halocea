/* _field_type_translated_index_parameter_verifier @0x8379B920 — field_parameters_verifier for the
 * "translated index" field type: validates maximum_active_at_once/initial_translation_table_size are both
 * positive, then (one-time, guarded by the field properties' own `initialized` flag) sets up the
 * translation table and the parallel translated-index allocation array (all slots -1 = free, slot 0 seeded
 * to 1 per the compiled initializer). */

#include <stdint.h>
#include <string.h>
#include "headers/field_properties_definition.h"
#include "headers/_field_type_translated_index_parameters.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void index_resolution_initialize(index_resolution_table *table, int number_of_slots);
extern void index_resolution_set(index_resolution_table *table, int server_object_index, int local_object_index);

uint8_t _field_type_translated_index_parameter_verifier(const _field_properties_definition *const field_properties_definition)
{
    _field_type_translated_index_parameters *parameters =
        (_field_type_translated_index_parameters *)field_properties_definition->parameters;

    if ( parameters->maximum_active_at_once < 1 )
        return 0;
    if ( parameters->initial_translation_table_size < 1 )
        return 0;

    if ( !field_properties_definition->initialized )
    {
        index_resolution_initialize(&parameters->translation_table, parameters->initial_translation_table_size);

        parameters->translated_index_allocations = dlMalloc(4 * parameters->maximum_active_at_once,
            "D:\\Projects\\code\\HCEX\\sources\\networking\\message_delta_definitions.c", 0x3D1u);
        parameters->translated_index_allocation_cursor = 0;
        memset(parameters->translated_index_allocations, -1, 4 * parameters->maximum_active_at_once);

        index_resolution_set(&parameters->translation_table, -1, 0);
        parameters->translated_index_allocations[0] = 1;
    }

    return 1;
}
