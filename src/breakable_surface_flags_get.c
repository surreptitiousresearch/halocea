/* breakable_surface_flags_get @ 0x83743108 — the destroyed-surface flag bitset for the active
 * structure BSP, consulted by collision so broken breakable surfaces no longer block rays. */

#include "headers/breakable_surface_globals.h"
#include <stdint.h>

const uint8_t *breakable_surface_flags_get(void)
{
    return globals->breakable_surface_flags[global_structure_bsp_index];
}
