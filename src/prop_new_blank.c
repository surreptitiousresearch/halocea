#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


extern int datum_new(data_array *data);
extern void prop_add(int actor_index, int unit_index, int prop_index);

int prop_new_blank(int actor_index)
{
    int prop_index = datum_new(prop_data);
    prop_add(actor_index, -1, prop_index);
    return prop_index;
}
