/* glow_normal_particle_update_size @0x8380BA68 — copies a float field at +0x20 to +0x24. Only
 * `object_index` is actually used and, despite its `int` type, is used as a raw address (confirmed via both
 * disasm and decompiler: `*(float *)(object_index + 36) = *(float *)(object_index + 32);`); `glow` and
 * `particle` are unread (no xrefs found to this function — likely reached only through a glow-update
 * function-pointer table, matching other glow_*_particle_update_* dispatch targets). Reproduced faithfully
 * per the DB prototype rather than reinterpreting object_index's true pointee type. */

#include "headers/glow_datum.h"
#include "headers/glow_particle.h"

void glow_normal_particle_update_size(int object_index, glow_datum *glow, glow_particle *particle)
{
    /* recovered: *(float *)(object_index+36) = *(float *)(object_index+32) -> present_size = initial_size
     * (offsets +0x24/+0x20 are glow_particle fields; object_index holds the particle pointer here — cf. the
     * inlined form `particle->present_size = particle->initial_size` in glow_update.c). Base pointer preserved. */
    glow_particle *size_particle = (glow_particle *)object_index;
    size_particle->present_size = size_particle->initial_size;
}
