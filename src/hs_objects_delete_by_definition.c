/* hs_objects_delete_by_definition @0x837F84D0 — delete every object instantiated from the given object
 * definition tag, then compact object memory. */

#include <stdint.h>
#include "headers/object_iterator.h"
#include "headers/object_type.h"

extern void object_delete(int object_index);
extern void objects_memory_compact(void);

void hs_objects_delete_by_definition(int definition_index)
{
    object_iterator iterator;
    object_iterator_new(&iterator, object_mask_all, 0);
    for ( uint32_t *object = object_iterator_next(&iterator); object; object = object_iterator_next(&iterator) )
    {
        if ( *object == definition_index )
            object_delete(iterator.index);
    }
    objects_memory_compact();
}
