/* surface_is_broken @0x837D2920 — true if a breakable surface has been broken: only meaningful for surfaces
 * with the "breakable" flag (bit 3) set, checked against the global breakable_surface_flags bit vector,
 * indexed by the surface's breakable_surface_index. Surface element layout (12 bytes: plane_designator@0,
 * flags@8, breakable_surface_index@9) per collision_bsp.h's own comment on `surfaces`. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/collision_bsp.h"
#include "headers/collision_surface.h"
#include "headers/collision_surface_flags.h"

extern const uint8_t *breakable_surface_flags_get(void);

uint8_t surface_is_broken(const structure_bsp *structure, int surface_index)
{
    collision_bsp *bsp = (collision_bsp *)structure->collision_bsp.address;
    collision_surface *surface = &((collision_surface *)bsp->surfaces.address)[surface_index];

    if ( surface->flags & (1u << _collision_surface_breakable_bit) )
    {
        unsigned __int8 breakable_surface_index = surface->breakable_surface_index;
        const char *flags = (const char *)breakable_surface_flags_get();
        unsigned int dword = *(const unsigned int *)(flags + (breakable_surface_index >> 3 & 0x1FFFFFFC));

        return ((1 << (breakable_surface_index & 0x1F)) & dword) == 0;
    }
    return 0;
}
