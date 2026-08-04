/* data_compact @0x836FB060 — compact a data_array in place: copy every occupied element (non-zero first
 * word, i.e. a live identifier) into a scratch buffer in order, write the compacted run back over the
 * array's storage, zero the freed tail, and update count/actual_count/first_free_absolute_index to the new
 * element count. No-op if the scratch allocation fails. */

#include "headers/data_array.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern void *memcpy(void *destination, const void *source, unsigned int size);
extern void *memset(void *destination, int value, unsigned int size);

void data_compact(data_array *data)
{
    char *scratch = dlMalloc(data->size * data->maximum_count, "D:\\Projects\\code\\HCEX\\sources\\memory\\data.c", 0x1F8u);
    if ( !scratch )
        return;

    __int16 compacted_count = 0;
    unsigned __int16 *element = (unsigned __int16 *)data->data;

    if ( data->count > 0 )
    {
        __int16 i = 0;
        do
        {
            if ( *element )
                memcpy(&scratch[compacted_count++ * data->size], element, data->size);
            i = (__int16)(i + 1);
            element = (unsigned __int16 *)((char *)element + data->size);
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
