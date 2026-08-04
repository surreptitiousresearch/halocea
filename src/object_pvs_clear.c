/* object_pvs_clear @0x836ECA60 — clear the scripted potentially-visible-set activation override. */

#include "headers/object_globals.h"
#include "headers/blam_data_globals.h"


void object_pvs_clear(void)
{
    object_globals->pvs_activation_type = 0;
}
