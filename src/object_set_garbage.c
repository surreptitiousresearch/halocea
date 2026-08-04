/* object_set_garbage @ 0x836EC7B0 — add or remove an object from the singly-linked garbage free list
 * (threaded through object_data+272, head at object_globals->first_garbage_object_index). Setting garbage
 * pushes the object on the list and sets flag 0x10000, but only if it is not already garbage and not
 * marked "do not garbage collect" (0x20000). Clearing garbage unlinks it and clears the flag. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_globals.h"
#include "headers/object_header_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


void object_set_garbage(int object_index, uint8_t garbage)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    int flags = object->object.flags;
    int is_garbage = flags & (1u << _object_garbage_bit);

    if ( garbage )
    {
        if ( !is_garbage && (flags & (1u << _object_cannot_be_garbage_bit)) == 0 )
        {
            object->object.next_garbage_object_index = object_globals->first_garbage_object_index;
            object_globals->first_garbage_object_index = object_index;
            object->object.flags |= (1u << _object_garbage_bit);
        }
    }
    else if ( is_garbage )
    {
        int *link;
        for ( link = &object_globals->first_garbage_object_index;
              *link != object_index;
              link = &DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, *link)
                          ->datum->object.next_garbage_object_index )
        {
            ;
        }
        *link = object->object.next_garbage_object_index;
        unsigned int cleared_flags = object->object.flags & ~(1u << _object_garbage_bit);
        object->object.next_garbage_object_index = -1;
        object->object.flags = cleared_flags;
    }
}
