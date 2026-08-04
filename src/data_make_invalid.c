/* data_make_invalid @0x836FAD08 — mark a data array invalid (not open for use). */
#include "headers/data_array.h"
void data_make_invalid(data_array *data)
{
    data->valid = 0;
}
