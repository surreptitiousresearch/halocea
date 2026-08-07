/* data_compact @0x836FB060 — compact a data_array in place: copy every occupied element (non-zero first
 * word, i.e. a live identifier) into a scratch buffer in order, write the compacted run back over the
 * array's storage, zero the freed tail, and update count/actual_count/first_free_absolute_index to the new
 * element count. No-op if the scratch allocation fails. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
/* memset declared by <string.h> */

void data_compact(data_array *data)
{
    char *scratch = dlMalloc(data->size * data->maximum_count, "D:\\Projects\\code\\HCEX\\sources\\memory\\data.c", 0x1F8u);
    if ( !scratch )
        return;

    int16_t compacted_count = 0;
    uint16_t *element = (uint16_t *)data->data;

    if ( data->count > 0 )
    {
        int16_t i = 0;
        do
        {
            if ( *element )
                memcpy(&scratch[compacted_count++ * data->size], element, data->size);
            i = (int16_t)(i + 1);
            element = (uint16_t *)((char *)element + data->size);
        }
        while ( i < data->count );
    }

    memcpy(data->data, scratch, data->size * compacted_count);
    memset((char *)data->data + compacted_count * data->size, 0, (data->maximum_count - compacted_count) * data->size);
    data->actual_count = compacted_count;
    data->count = compacted_count;
    data->first_free_absolute_index = compacted_count;
    dlFree(scratch);
}
