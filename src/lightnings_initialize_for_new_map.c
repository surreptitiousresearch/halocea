#include "headers/lightning_globals.h"
#include "headers/data_array.h"

void lightnings_initialize_for_new_map(void)
{
    if (lightning_globals.lightning_data)
        data_make_valid(lightning_globals.lightning_data);
}
