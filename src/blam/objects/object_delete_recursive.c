/* object_delete_recursive @ 0x836F1F80 — second pass of object teardown: structurally free the object
 * and (recursively) its attached children/siblings. For each object: drop it from the garbage list if
 * flagged garbage, fire the deleted-procs callbacks, recurse into the first child (first_child_object_index)
 * and, when delete_siblings is set, the next sibling (next_object_index), clear the active bit, delete the
 * type-specific data (widgets, attachments, optional map disconnect if connected-to-map, then object_type_delete), free the
 * object data block back to object_memory_pool, retire the datum, and null the header's data pointer
 * and flags byte. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/memory_pool.h"
#include "headers/object_header_datum.h"
#include "headers/object_flags.h"
#include "headers/object_header_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/object_type_definition.h"
extern void object_set_garbage(int object_index, uint8_t garbage);
extern void object_deleted_procs_call(int deleted_object_index);
extern object_type_definition *object_type_definition_get(int16_t object_type);
extern void widgets_delete(int object_index);
extern void attachments_delete(int object_index);
extern void object_disconnect_from_map(int object_index);
extern void object_type_delete(int object_index);
extern void memory_pool_block_free(memory_pool *pool, void **reference);
extern void datum_delete(data_array *data, int index);

void object_delete_recursive(int object_index, uint8_t delete_siblings)
{
    object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *const object_data = object_header->datum;

    if ( (object_data->object.flags & (1u << _object_garbage_bit)) != 0 )
        object_set_garbage(object_index, 0);
    object_deleted_procs_call(object_index);

    int first_child = object_data->object.first_child_object_index;
    if ( first_child != -1 )
        object_delete_recursive(first_child, 1u);
    if ( delete_siblings )
    {
        int next_sibling = object_data->object.next_object_index;
        if ( next_sibling != -1 )
            object_delete_recursive(next_sibling, 1u);
    }

    if ( (object_header->flags & (1u << _object_header_active_bit)) != 0 )
        object_header->flags &= ~(1u << _object_header_active_bit);

    object_type_definition_get((uint16_t)object_header->datum->object.type);
    widgets_delete(object_index);
    attachments_delete(object_index);
    if ( (object_data->object.flags & (1u << _object_connected_to_map_bit)) != 0 )
        object_disconnect_from_map(object_index);
    object_type_delete(object_index);

    if ( object_header->datum )
        memory_pool_block_free(object_memory_pool, &object_header->datum);
    datum_delete(object_header_data, object_index);
    object_header->datum = 0;
    object_header->flags = 0;
}
