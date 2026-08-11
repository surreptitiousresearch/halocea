/* message_definition_field_reference_set_dispose @0x837A1DB8 — tears down every initialized field reference
 * in a header/body field-reference set: for any field whose type's properties were themselves initialized,
 * invokes that field type's destructor callback, then clears both initialized flags. */

#include "headers/message_definition_field_reference_set.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


void message_definition_field_reference_set_dispose(_message_definition_field_reference_set *const field_reference_set)
{
    for ( int i = 0; i < field_reference_set->count; ++i )
    {
        _field_reference_definition *field_reference = &field_reference_set->field_references[i];
        if ( field_reference->initialized == 1 )
        {
            _field_properties_definition *properties = field_reference->properties;
            if ( properties->initialized == 1 )
            {
                message_delta_global_field_type_list[properties->type].destructor(properties);
                properties->initialized = 0;
            }
            field_reference->initialized = 0;
        }
    }
}
