/* surface_is_broken @0x837D2910 — true if a breakable surface has been broken: only meaningful for surfaces
 * with the "breakable" flag (bit 3) set, checked against the global breakable_surface_flags bit vector,
 * indexed by the surface's breakable_surface_index. Surface element layout (12 bytes: plane_designator@0,
 * flags@8, breakable_surface_index@9) per collision_bsp.h's own comment on `surfaces`. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/collision_bsp.h"
#include "headers/collision_surface.h"
#include "headers/collision_surface_flags.h"
#include "headers/bit_vector.h"

extern const uint8_t *breakable_surface_flags_get(void);

uint8_t surface_is_broken(const structure_bsp *structure, int surface_index)
{
    collision_bsp *bsp = (collision_bsp *)structure->collision_bsp.address;
    collision_surface *surface = &((collision_surface *)bsp->surfaces.address)[surface_index];

    if ( surface->flags & (1u << _collision_surface_breakable_bit) )
    {
        uint8_t breakable_surface_index = surface->breakable_surface_index;
        const unsigned int *flags = (const unsigned int *)breakable_surface_flags_get();

        /* DEVIATION: the decompiler byte-addressed the word as `*(unsigned int *)(flags +
         * (i >> 3 & 0x1FFFFFFC))`; `(i >> 3) & ~3` IS `(i >> 5) * 4`, i.e. the plain cseries
         * bit-vector word index. A surface is broken when its flag is CLEAR. */
        return !BIT_VECTOR_TEST_FLAG(flags, breakable_surface_index);
    }
    return 0;
}
