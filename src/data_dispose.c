/* data_dispose @0x836FACC8 — zero a data_array header and free it. */

#include "headers/data_array.h"
#include <string.h>

extern void dlFree(void *ptr);

void data_dispose(data_array *data)
{
    memset(data, 0, sizeof(data_array));
    dlFree(data);
}
