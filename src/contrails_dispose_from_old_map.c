/* contrails_dispose_from_old_map @ 0x8373FB18 — invalidate contrail pools */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern void data_make_invalid(data_array *data);
void contrails_dispose_from_old_map(void)
{
    data_make_invalid(contrail_point_data);
    data_make_invalid(contrail_data);
}
