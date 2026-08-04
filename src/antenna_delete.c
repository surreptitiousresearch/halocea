#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

void antenna_delete(int antenna_index)
{
    datum_delete(antenna_data, antenna_index);
}
