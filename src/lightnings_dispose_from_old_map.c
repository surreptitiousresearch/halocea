#include "headers/lightning_globals.h"
#include "headers/data_array.h"

void lightnings_dispose_from_old_map(void)
{
    if (lightning_globals.lightning_data)
        data_make_invalid(lightning_globals.lightning_data);
}
