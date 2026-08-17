/* field_reference_dispose @0x837A1148 */
#include "headers/field_reference_definition.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


void field_reference_dispose(_field_reference_definition *const field_reference_definition)
{
    if ( field_reference_definition->initialized == 1 )
    {
        _field_properties_definition *properties = field_reference_definition->properties;

        if ( properties->initialized == 1 )
        {
            message_delta_global_field_type_list[properties->type].destructor(properties);
            properties->initialized = 0;
        }
        field_reference_definition->initialized = 0;
    }
}
