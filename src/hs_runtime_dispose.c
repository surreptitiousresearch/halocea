#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_invalid(data_array *data);

void hs_runtime_dispose(void)
{
    data_make_invalid(hs_global_data);
}
