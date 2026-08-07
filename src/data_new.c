/* data_new @ 0x836FB1E8
   Allocates and initializes a data_array: header (56 bytes) plus inline storage
   for maximum_count elements of `size` bytes. Storage immediately follows the
   header. The signature 0x64407440 ('d@t@') marks a live array. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
/* strncpy declared by <string.h> */

data_array *data_new(const char *name, int16_t maximum_count, int16_t size)
{
    data_array *array = dlMalloc(maximum_count * size + 56,
        "D:\\Projects\\code\\HCEX\\sources\\memory\\data.c", 0x31);
    if ( array )
    {
        memset(array, 0, sizeof(data_array));
        strncpy(array->name, name, 0x1F);
        array->maximum_count = maximum_count;
        array->size = size;
        array->data = &array[1];        /* storage follows the header */
        array->signature = 0x64407440u; /* 'd@t@' live-array tag */
        array->valid = 0;
    }
    return array;
}
