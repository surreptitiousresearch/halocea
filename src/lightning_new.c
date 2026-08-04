#include "headers/lightning_globals.h"

#include "headers/data_array.h"
extern int datum_new(data_array *data);
extern void *datum_get(data_array *data, int index);

int lightning_new(int definition_index)
{
    int index = datum_new(lightning_globals.lightning_data);

    if (index != -1)
        *((int *)datum_get(lightning_globals.lightning_data, index) + 1) = definition_index;

    return index;
}
