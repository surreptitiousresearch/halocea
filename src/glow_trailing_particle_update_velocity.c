/* glow_trailing_particle_update_velocity @0x8380B74C — update a trailing glow particle's velocity: if the
 * glow definition's "fade over lifetime" flag (definition+40, bit 0x8 — same flag as
 * glow_trailing_particle_update_color.c) is set, present_velocity = initial_velocity scaled by the
 * remaining-lifetime fraction (1 - ticks_in_existence/lifetime, clamped to >= 0, no upper clamp here);
 * otherwise present_velocity is just a straight copy of initial_velocity.
 *
 * DEVIATION: carries a "local variable allocation has failed" warning; the decompiler lost `particle` as a
 * real pointer entirely, rendering it as a 64-bit scratch value with HIDWORD/LODWORD field-offset math.
 * Disasm (0x8380B74C-0x8380B7DC) confirms r4 (particle) is used directly throughout at ordinary offsets
 * (+0x38/+0x3C/+0x40 initial_velocity, +0x44/+0x48/+0x4C present_velocity, +0x50/+0x52
 * ticks_in_existence/lifetime) — restored using glow_particle.h's named fields. */

#include "headers/glow_datum.h"
#include "headers/glow_particle.h"
#include "headers/glow_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/glow_definition_flags.h"

void glow_trailing_particle_update_velocity(glow_datum *glow, glow_particle *particle)
{
    glow_definition *definition = TAG_GET(glow_definition, glow->definition_index);
    unsigned __int8 fades_over_lifetime = (definition->flags & (1u << _glow_trailing_particles_fade_over_time_bit)) != 0;

    if ( fades_over_lifetime )
    {
        float fraction = 1.0f - (float)particle->ticks_in_existence / (float)particle->lifetime;
        if ( fraction < 0.0f )
            fraction = 0.0f;

        particle->present_velocity.n[0] = particle->initial_velocity.n[0] * fraction;
        particle->present_velocity.n[1] = particle->initial_velocity.n[1] * fraction;
        particle->present_velocity.n[2] = particle->initial_velocity.n[2] * fraction;
    }
    else
    {
        particle->present_velocity = particle->initial_velocity;
    }
}
