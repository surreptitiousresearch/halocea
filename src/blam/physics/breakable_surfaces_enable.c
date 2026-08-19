/* breakable_surfaces_enable @0x837430F0 — globally enable or disable destructible surfaces. */

#include <stdint.h>
#include "headers/breakable_surface_globals.h"

void breakable_surfaces_enable(uint8_t enable)
{
    globals->enabled = enable;
}
