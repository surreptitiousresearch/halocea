/* object_destroy_notify_children @0x836B1500 — when an object is destroyed, notify each child (and,
 * recursively, their children) so they can react (e.g. detach). Each child that doesn't handle the
 * parent-destroyed event itself has its own children notified. Walks first_child_object_index and
 * the next_object_index sibling chain. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t object_type_handle_parent_destroyed(int object_index);

void object_destroy_notify_children(int object_index)
{
    int child_index = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum)->object.first_child_object_index;
    while ( child_index != -1 )
    {
        int next_sibling = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child_index)->datum)->object.next_object_index;
        if ( !object_type_handle_parent_destroyed(child_index) )
            object_destroy_notify_children(child_index);
        child_index = next_sibling;
    }
}
