/* data_initialize @ 0x836FAC60
   Initializes a data_array in place (header + inline storage following it).
   Same field setup as data_new but without allocating. */

#include <stdint.h>

#include "headers/data_array.h"

extern void *memset(void *dst, int value, unsigned int size);
extern char *strncpy(char *dst, const char *src, unsigned int count);

void data_initialize(data_array *data, const char *name, int16_t maximum_count, int16_t size)
{
    memset(data, 0, sizeof(data_array));
    strncpy(data->name, name, 0x1F);
    data->maximum_count = maximum_count;
    data->size = size;
    data->data = &data[1];          /* storage follows the header */
    data->signature = 0x64407440u;  /* 'd@t@' live-array tag */
    data->valid = 0;
}
