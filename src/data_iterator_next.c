/* data_iterator_next @0x836FADE8 — advance a data_array cursor to the next in-use datum and return it, or null
 * at the end. A datum is in-use when its leading __int16 identifier word is nonzero. The packed index stored in
 * iterator->index is (identifier << 16) | absolute_index. */

#include "headers/data_iterator.h"
#include "headers/datum_index.h"

void *data_iterator_next(data_iterator *iterator)
{
    __int16 absolute_index = iterator->absolute_index;
    __int16 count = iterator->data->count;
    int     size = iterator->data->size;
    int     current_index = absolute_index;
    char   *base = (char *)iterator->data->data;
    char   *datum = &base[absolute_index * (__int16)size];
    char   *result = nullptr;
    int     packed_index;

    if ( absolute_index < count )
    {
        while ( 1 )
        {
            absolute_index = current_index + 1;
            packed_index = BUILD_DATUM_INDEX(*(__int16 *)datum, current_index);
            if ( *(unsigned __int16 *)datum )
                break;
            current_index = absolute_index;
            datum += size;
            if ( absolute_index >= iterator->data->count )
            {
                iterator->absolute_index = absolute_index;
                return result;
            }
        }
        iterator->index = packed_index;
        result = datum;
    }
    iterator->absolute_index = absolute_index;
    return result;
}
