/* breakable_surfaces_initialize_for_new_map @0x83743040 — enable breakable surfaces and, for every BSP, clear
 * the destroyed-surface flags and reset every surface's vitality to full (1.0).
 *
 * Deviation: the decompiler addresses the vitality table with raw arithmetic (&enabled + 4*(bsp*256 + i + 129));
 * reproduced as breakable_surfaces[bsp][i] indexing — offset 0x204 = float index 129. */

#include "headers/breakable_surface_globals.h"
extern void *memset(void *dst, int value, unsigned int n);

void breakable_surfaces_initialize_for_new_map(void)
{
    struct breakable_surface_globals *g = globals;
    g->enabled = 1;
    for ( __int16 bsp = 0; bsp < 16; bsp = (__int16)(bsp + 1) )
    {
        memset(g->breakable_surface_flags[bsp], -1, sizeof(g->breakable_surface_flags[bsp]));
        for ( __int16 surface = 0; surface < 256; surface = (__int16)(surface + 1) )
            g->breakable_surfaces[bsp][surface].vitality = 1.0f;
    }
}
