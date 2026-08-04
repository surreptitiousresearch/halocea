/* contrails_dispose @ 0x8373FB18 — drop contrail + contrail-point pool pointers */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
void contrails_dispose(void)
{
    if ( contrail_point_data ) contrail_point_data = 0;
    if ( contrail_data )       contrail_data = 0;
}
