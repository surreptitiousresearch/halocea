/* object_iterator_next @0x836EC5D0 — advance an object-table cursor to the next live object whose type is in
 * the iterator's type mask and whose object flags include the iterator's flag mask. Returns the object data
 * pointer (and records the full datum handle in iterator->index), or null at the end of the table.
 *
 * Each object header record is 12 bytes: halfword[0] = identifier (0 = empty slot), byte[2] = object flags
 * (matched against iterator->flags), byte[3] = object type (1<<type matched against type_flags), and
 * dword[2] = the object data pointer. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_iterator.h"
#include "headers/blam_data_globals.h"
#include "headers/datum_index.h"


void *object_iterator_next(object_iterator *iterator)
{
    __int16 absolute_index = iterator->absolute_index;
    int scan_index = absolute_index;
    void *result = 0;
    int count = object_header_data->count;
    object_header_datum *record = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, absolute_index);

    if ( absolute_index < count )
    {
        int datum_handle;
        while ( 1 )
        {
            absolute_index = scan_index + 1;
            datum_handle = BUILD_DATUM_INDEX(record->identifier, scan_index);
            if ( record->identifier
              && (unsigned __int8)(record->flags & iterator->flags) == iterator->flags
              && ((1 << record->type) & iterator->type_flags) != 0 )
                break;

            scan_index = absolute_index;
            ++record;
            if ( absolute_index >= count )
            {
                iterator->absolute_index = absolute_index;
                return result;
            }
        }
        iterator->index = datum_handle;
        result = record->datum;
    }

    iterator->absolute_index = absolute_index;
    return result;
}
