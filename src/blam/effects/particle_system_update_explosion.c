/* particle_system_update_explosion @0x8373B6F0 — tail-call forwarder
 * (`b particle_system_update_default`; confirmed via disasm, not a bare blr). The "explosion" particle
 * system update variant collapsed to the default update in the retail build. particle_system_update_
 * default itself is not yet decompiled — declared extern with its own DB prototype and called through
 * with the same arguments. */

#include "headers/particle_system_datum.h"

extern void particle_system_update_default(particle_system_datum *system, float dtime);

void particle_system_update_explosion(particle_system_datum *system, float dtime)
{
    particle_system_update_default(system, dtime);
}
