#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

void flag_delete(int flag_index)
{
    datum_delete(flag_data, flag_index);
}
