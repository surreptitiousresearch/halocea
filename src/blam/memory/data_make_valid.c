/* data_make_valid @ 0x836FB278
   Marks the array valid and clears it to the empty state. */

#include "headers/data_array.h"

extern void data_delete_all(data_array *data);

void data_make_valid(data_array *data)
{
    data->valid = 1;
    data_delete_all(data);
}
