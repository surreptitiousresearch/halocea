/* data_delete_all @0x836FAD40 — empty a data array in place: reset the bookkeeping counters, reseed the
 * identifier counter from the array name (high bit set), and clear every datum's identifier word so all
 * slots read as free. */

#include "headers/data_array.h"
#include <string.h>

void data_delete_all(data_array *data)
{
    data->count = 0;
    data->actual_count = 0;
    data->first_free_absolute_index = 0;
    strncpy((char *)&data->next_identifier, data->name, 2u);
    data->next_identifier |= 0x8000u;
    int maximum_count = data->maximum_count;
    if ( maximum_count > 0 )
    {
        for ( int index = 0; index < data->maximum_count; index = (short)(index + 1) )
            DATA_ARRAY_IDENTIFIER(data, index) = 0;
    }
}
