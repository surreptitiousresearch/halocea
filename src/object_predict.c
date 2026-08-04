/* object_predict @0x836ECB78 — precache the predicted resources for an object and all of its children:
 * walk the next-object chain (object dword +69), and for each, precache its definition's
 * predicted-resources block (tag_block @ definition +368) and recurse into its first child (dword +70). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/tag_block.h"
#include "headers/_object_definition.h"
#include "headers/blam_data_globals.h"

#include "headers/tag_block.h"
extern void predicted_resources_precache(const tag_block *predicted_resources);

void object_predict(int object_index)
{
    while ( object_index != -1 )
    {
        object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        if ( object->definition_index != -1 )
        {
            _object_definition *definition = TAG_GET(_object_definition, object->definition_index);
            predicted_resources_precache(&definition->predicted_resources);
        }
        object_predict(object->object.first_child_object_index);
        object_index = object->object.next_object_index;
    }
}
