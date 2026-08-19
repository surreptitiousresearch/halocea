/* objects_scripting_detach @0x836F14B8 — detach a child object from its parent, but only if it is currently
 * parented to the specified parent (object data +0x11C == parent). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"

extern void object_detach(int child_object_index);

void objects_scripting_detach(int parent_object_index, int child_object_index)
{
    if ( parent_object_index == -1 || child_object_index == -1 )
        return;
    object_datum *child_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child_object_index)->datum;
    if ( child_data->object.parent_object_index == parent_object_index )
        object_detach(child_object_index);
}
