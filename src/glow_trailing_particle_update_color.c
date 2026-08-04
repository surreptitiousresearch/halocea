/* glow_trailing_particle_update_color @0x8380B614 — update a trailing glow particle's fade: if the glow
 * definition's "fade over lifetime" flag (definition+40, bit 0x8) is set, fade = 1.0 - ticks_in_existence /
 * lifetime, clamped to [0,1]; otherwise fade stays at 1.0 (fully opaque, no fade-out).
 *
 * DEVIATION: the decompiler's "local variable allocation has failed" warning reflects it losing track of
 * `particle` as a real pointer, instead treating it as a 64-bit scratch value reused to hold both the
 * ticks_in_existence field (via a stack std/lfd round-trip artifact) and the lifetime field. Disasm
 * (0x8380B614-0x8380B694) confirms particle+0x50/+0x52/+0x58 are read/written directly, matching
 * ticks_in_existence/lifetime/fade in glow_particle.h exactly. */

#include "headers/glow_datum.h"
#include "headers/glow_particle.h"
#include "headers/glow_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/glow_definition_flags.h"

void glow_trailing_particle_update_color(glow_datum *glow, glow_particle *particle)
{
    glow_definition *definition = TAG_GET(glow_definition, glow->definition_index);
    unsigned __int8 fades_over_lifetime = (definition->flags & (1u << _glow_trailing_particles_fade_over_time_bit)) != 0;

    if ( !fades_over_lifetime )
    {
        particle->fade = 1.0f;
        return;
    }

    float fade = 1.0f - (float)particle->ticks_in_existence / (float)particle->lifetime;

    if ( fade < 0.0f )
        fade = 0.0f;
    else if ( fade > 1.0f )
        fade = 1.0f;

    particle->fade = fade;
}
