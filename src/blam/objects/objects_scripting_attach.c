/* objects_scripting_attach @0x836F20C8 — attach a child object's marker to a parent object's marker, but only
 * if the child is not already parented (object data +0x11C == -1). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"

extern void object_attach_to_marker(int parent_object_index, const char *parent_marker_name, int child_object_index, const char *child_marker_name);

void objects_scripting_attach(int parent_object_index, const char *parent_marker_name,
                              int child_object_index, const char *child_marker_name)
{
    if ( parent_object_index == -1 || child_object_index == -1 )
        return;
    object_datum *child_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child_object_index)->datum;
    if ( child_data->object.parent_object_index == -1 )
        object_attach_to_marker(parent_object_index, parent_marker_name, child_object_index, child_marker_name);
}
