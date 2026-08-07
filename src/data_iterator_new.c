/* data_iterator_new @0x836FADC0 — initialize a cursor over a data_array. The signature salts the array pointer
 * so a stale iterator can be detected by data_iterator_next. */

#include "headers/data_iterator.h"

void data_iterator_new(data_iterator *iterator, data_array *data)
{
    iterator->data = data;
    iterator->absolute_index = 0;
    iterator->index = -1;
    iterator->signature = (unsigned int)(uintptr_t)data ^ 0x69746572u; /* 'iter' */
}
