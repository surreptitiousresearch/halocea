/* object_delete_initial_recursive @ 0x836F06F0 — first pass of object teardown: walk the attachment
 * tree (first child, and siblings when delete_siblings is set), mark each object's header
 * "pending delete" flag (0x8), hide it, and release its scenario object-name slot. Clearing the
 * name index to 0xFFFF and scrubbing any matching entry in object_name_list keeps scripts from
 * resolving a deleted object by name. The structural free happens later in object_delete_recursive. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"


extern void object_set_visibility(int object_index, uint8_t visible_flag);

extern void object_name_list_delete(int object_index);
void object_delete_initial_recursive(int object_index, uint8_t delete_siblings)
{
    object_header_datum *header =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object = header->datum;

    int first_child = object->object.first_child_object_index;
    if ( first_child != -1 )
        object_delete_initial_recursive(first_child, 1u);
    if ( delete_siblings )
    {
        int next_sibling = object->object.next_object_index;
        if ( next_sibling != -1 )
            object_delete_initial_recursive(next_sibling, 1u);
    }

    header->flags |= (1u << _object_header_being_deleted_bit);
    object_set_visibility(object_index, 0);

    /* DEVIATION: collapsed inlined-clone of object_name_list_delete@0x836EE0D0 (zero-xref donor); object_index passed through unchanged, no folded args. */
    object_name_list_delete(object_index);
}
