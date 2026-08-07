/* breakable_surfaces_initialize_for_new_map @0x83743040 — enable breakable surfaces and, for every BSP, clear
 * the destroyed-surface flags and reset every surface's vitality to full (1.0).
 *
 * Deviation: the decompiler addresses the vitality table with raw arithmetic (&enabled + 4*(bsp*256 + i + 129));
 * reproduced as breakable_surfaces[bsp][i] indexing — offset 0x204 = float index 129. */

#include <stdint.h>
#include <string.h>
#include "headers/breakable_surface_globals.h"

void breakable_surfaces_initialize_for_new_map(void)
{
    struct breakable_surface_globals *g = globals;
    g->enabled = 1;
    for ( int16_t bsp = 0; bsp < 16; bsp = (int16_t)(bsp + 1) )
    {
        memset(g->breakable_surface_flags[bsp], -1, sizeof(g->breakable_surface_flags[bsp]));
        for ( int16_t surface = 0; surface < 256; surface = (int16_t)(surface + 1) )
            g->breakable_surfaces[bsp][surface].vitality = 1.0f;
    }
}
