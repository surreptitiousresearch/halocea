#include "headers/light_volume_globals.h"

#include "headers/data_array.h"
extern int datum_new(data_array *data);
extern void *datum_get(data_array *data, int index);

int light_volume_new(int definition_index)
{
    int index = datum_new(light_volume_globals.light_volume_data);

    if (index != -1)
        *((int *)datum_get(light_volume_globals.light_volume_data, index) + 1) = definition_index;

    return index;
}
