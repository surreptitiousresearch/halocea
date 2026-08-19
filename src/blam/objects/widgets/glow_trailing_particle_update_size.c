/* glow_trailing_particle_update_size @0x8380B6A0 — if the glow tag definition's flags word (dword index 10,
 * i.e. +0x28) has bit 0x10 set, fades a trailing particle's size out linearly over its lifetime:
 * present_size = max(1.0 - ticks_in_existence/lifetime, 0.0) * initial_size.
 *
 * DEVIATION: the decompiler mis-typed `particle` as a raw __int64 and flagged its own output unreliable
 * ("local variable allocation has failed"); disasm_range(0x8380B6A0,0x8380B724) was used to recover the
 * true register mapping and confirm the DB's 2-pointer-argument prototype. */

#include "headers/glow_datum.h"
#include "headers/glow_particle.h"
#include "headers/glow_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/glow_definition_flags.h"

void glow_trailing_particle_update_size(glow_datum *glow, glow_particle *particle)
{
    glow_definition *tag_definition = TAG_GET(glow_definition, glow->definition_index);

    if ( (tag_definition->flags & (1u << _glow_trailing_particles_shrink_over_time_bit)) != 0 )
    {
        float remaining = 1.0f - (float)particle->ticks_in_existence / (float)particle->lifetime;

        if ( remaining < 0.0f )
            remaining = 0.0f;

        particle->present_size = remaining * particle->initial_size;
    }
}
