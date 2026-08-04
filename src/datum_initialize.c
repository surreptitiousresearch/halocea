/* datum_initialize @0x836FB1A4 — initialize a freshly-allocated datum: zero its record and stamp it
 * with the array's next identifier, advancing (and wrapping past -1 to 0x8000) the identifier counter. */

#include "headers/data_array.h"
#include "headers/datum_header.h"
#include <string.h>

void datum_initialize(data_array *data, datum_header *header)
{
    memset(header, 0, data->size);
    header->identifier = data->next_identifier;
    short next_identifier = data->next_identifier;
    data->next_identifier = next_identifier + 1;
    if ( next_identifier == -1 )
        data->next_identifier = 0x8000;
}
