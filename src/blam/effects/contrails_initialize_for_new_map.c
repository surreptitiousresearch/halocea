/* contrails_initialize_for_new_map @0x8373EFB8 — revalidate the contrail and contrail-point pools. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);

void contrails_initialize_for_new_map(void)
{
    data_make_valid(contrail_data);
    data_make_valid(contrail_point_data);
}
