/* object_compute_node_matrices_recursive @0x836F13C0 — compute node matrices for an object and then for every
 * object attached to it, walking the child/sibling object chain. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


extern void object_compute_node_matrices(int object_index);

void object_compute_node_matrices_recursive(int object_index)
{
    object_datum *object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index))->datum;
    object_compute_node_matrices(object_index);
    for ( int child = object->object.first_child_object_index; child != -1; )
    {
        object_datum *child_object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child))->datum;
        object_compute_node_matrices_recursive(child);
        child = child_object->object.next_object_index;
    }
}
