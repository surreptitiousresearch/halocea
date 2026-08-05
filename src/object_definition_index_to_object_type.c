/* object_definition_index_to_object_type @0x83702AD8 — resolve a tag's group_tag to its object_type enum
 * index by scanning the 12-entry object_type_definitions table. -1 if no type owns that group tag. */

#include <stdint.h>
#include "headers/object_type_definition.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern uint32_t tag_get_group_tag(int16_t tag_index);

int16_t object_definition_index_to_object_type(int definition_index)
{
    unsigned int group_tag = tag_get_group_tag(definition_index);

    for ( int16_t object_type = object_type_biped; object_type < number_of_object_types; ++object_type )
    {
        if ( object_type_definitions[object_type]->group_tag == group_tag )
            return object_type;
    }
    return -1;
}
