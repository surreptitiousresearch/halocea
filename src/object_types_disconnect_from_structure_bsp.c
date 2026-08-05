#include <stdint.h>
#include "headers/object_iterator.h"
#include "headers/object_datum.h"
#include "headers/object_type.h"

extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);
extern void object_delete(int object_index);

void object_types_disconnect_from_structure_bsp(void)
{
    object_iterator iterator;
    object_datum *object;

    object_iterator_new(&iterator, object_mask_remove_on_bsp_switch, 0);
    for ( object = object_iterator_next(&iterator); object;
          object = object_iterator_next(&iterator) )
    {
        /* name_index is a signed __int16; keep the unsigned cast so the 0xFFFF sentinel compare holds */
        if ( (uint16_t)object->object.name_index == 0xFFFF )
            object_delete(iterator.index);
    }
}
