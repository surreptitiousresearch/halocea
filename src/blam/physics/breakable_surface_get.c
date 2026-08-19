/* breakable_surface_get @0x83742FD0 — the breakable-surface datum for the current structure BSP. */

#include <stdint.h>
#include "headers/breakable_surface_globals.h"
#include "headers/breakable_surface_datum.h"

breakable_surface_datum * breakable_surface_get(int16_t breakable_surface_index)
{
    return &globals->breakable_surfaces[global_structure_bsp_index][breakable_surface_index];
}
