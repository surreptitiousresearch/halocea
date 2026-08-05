#include <stdint.h>
#include "headers/blam_data_globals.h"
/* global_structure_bsp_index_get @0x83704048 — return the index of the currently-loaded structure BSP. */


int global_structure_bsp_index_get(void)
{
    return (uint16_t)global_structure_bsp_index;
}
