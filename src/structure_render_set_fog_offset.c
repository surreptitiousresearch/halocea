/* structure_render_set_fog_offset @0x837C62A8 — latch a world-space fog plane offset into the structure render
 * globals for this frame, marking it valid. */

#include "headers/structure_render_globals.h"
#include "headers/real_vector3d.h"

void structure_render_set_fog_offset(real_vector3d *vector_offset)
{
    structure_render_globals.fog_offset_valid = 1;
    structure_render_globals.fog_offset_vector = *vector_offset;
}
