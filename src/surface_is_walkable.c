/* surface_is_walkable @0x8381E9B0 */
#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/collision_surface.h"
#include "headers/PATHFINDING_SURFACE_flags.h"
#include "headers/bit_vector.h"

BOOL surface_is_walkable(const collision_bsp *bsp, const uint8_t *pathfinding_surface_data, uint8_t ignore_broken_surfaces, const uint8_t *breakable_surface_flags, int surface_index)
{
    unsigned int flags = pathfinding_surface_data[surface_index];
    BOOL walkable = (flags >> PATHFINDING_SURFACE_WALKABLE_BIT) & 1;

    if ( !ignore_broken_surfaces && walkable && (flags & (1u << PATHFINDING_SURFACE_BREAKABLE_BIT)) )
    {
        /* breakable_surface_index lives in the collision_surface element (offset 0x09), read via the
         * typed element rather than raw 12*index+9 byte math — precedent: surface_is_broken. */
        unsigned int breakable_surface_index =
            ((const collision_surface *)bsp->surfaces.address)[surface_index].breakable_surface_index;

        /* DEVIATION: decompiler's byte-offset word pun ((i >> 3) & ~3) rewritten as the cseries
         * 32-bit-word bit-vector test (identical word/bit split). */
        return BIT_VECTOR_TEST_FLAG((const uint32_t *)breakable_surface_flags, breakable_surface_index);
    }
    return walkable;
}
