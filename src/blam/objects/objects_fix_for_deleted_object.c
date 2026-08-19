/* objects_fix_for_deleted_object @0x836EC718 — after an object is deleted, walk every live object and
 * clear any dangling reference to the deleted index (its umbrella-shield object reference), then give each
 * object type a chance to fix up its own state. */

#include "headers/object_iterator.h"
#include "headers/object_datum.h"

extern void object_type_handle_deleted_object(int object_index, int deleted_object_index);

void objects_fix_for_deleted_object(int deleted_object_index)
{
    object_iterator iterator;
    iterator.flags = 0;
    iterator.absolute_index = 0;
    iterator.signature = 0x86868686;
    iterator.type_flags = (unsigned int)-1;
    iterator.index = -1;

    for ( object_datum *object = object_iterator_next(&iterator);
          object;
          object = object_iterator_next(&iterator) )
    {
        if ( object->object.umbrella_shield_object_index == deleted_object_index )
            object->object.umbrella_shield_object_index = -1;
        object_type_handle_deleted_object(iterator.index, deleted_object_index);
    }
}
