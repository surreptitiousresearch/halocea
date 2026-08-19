/* _field_type_arbitrary_data_size_calculator @0x8379AE58 */
#include "headers/field_properties_definition.h"

int _field_type_arbitrary_data_size_calculator(_field_properties_definition *const field_properties_definition)
{
    return 8 * *(int *)field_properties_definition->parameters;
}
