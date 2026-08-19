/* object_iterator_new @0x836EC5A8 — initialize an object-table cursor: store the requested object-type mask
 * and flag mask, reset the scan position, stamp the iterator signature and clear the current index. */

#include <stdint.h>
#include "headers/object_iterator.h"

void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags)
{
    iterator->type_flags = type_flags;
    iterator->flags = flags;
    iterator->absolute_index = 0;
    iterator->signature = 0x86868686u;  /* iterator-live sentinel (byte 0x86 repeated); not a fourcc */
    iterator->index = -1;
}
