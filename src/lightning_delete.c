#include "headers/lightning_globals.h"
#include "headers/data_array.h"

extern void datum_delete(data_array *data, int index);

void lightning_delete(int lightning_index)
{
    if (lightning_index != -1)
        datum_delete(lightning_globals.lightning_data, lightning_index);
}
