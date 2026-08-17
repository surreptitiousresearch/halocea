/* get_object_icon_text_index @0x83707F50 */
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/_object_definition.h"
#include "headers/blam_data_globals.h"


int get_object_icon_text_index(int object_index)
{
    if ( object_index == -1 )
        return -1;

    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    int definition_index = object->definition_index;

    return (TAG_GET(_object_definition, definition_index))->icon_text_index;   /* objdef+316 */
}
