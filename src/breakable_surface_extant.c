/* breakable_surface_extant @0x83743130 */
#include <stdint.h>
#include "headers/breakable_surface_globals.h"

uint8_t breakable_surface_extant(int16_t breakable_surface_index)
{
    if (breakable_surface_index == -1)
        return 1;

    unsigned char *flags = globals->breakable_surface_flags[global_structure_bsp_index];
    unsigned int   word  = *(unsigned int *)&flags[4 * (breakable_surface_index >> 5)];
    return (word & (1 << (breakable_surface_index & 0x1F))) != 0;
}
