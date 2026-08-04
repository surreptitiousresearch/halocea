#include <stdint.h>
#include "headers/blam_data_globals.h"

void debug_pvs(uint8_t on)
{
    debug_portals          = on;
    structures_use_pvs_for_vs = on;
}
