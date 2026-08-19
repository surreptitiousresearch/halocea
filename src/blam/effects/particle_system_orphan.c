/* particle_system_orphan @ 0x83739BC8 — detach a particle system from its owner object so it finishes
 * playing on its own and is reclaimed once dead. Clears the owner reference (system+12 = -1) and the
 * _particle_system_active_bit (bit0). Particle system datum stride is 344 bytes. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/particle_system_datum.h"
#include "headers/particle_system_flags.h"
#include "headers/blam_data_globals.h"


void particle_system_orphan(int system_index)
{
    particle_system_datum *system = DATA_ARRAY_ELEMENT(particle_systems, particle_system_datum, system_index);
    system->object_index = -1;                     /* owner object index → none */
    system->flags &= ~(1u << _particle_system_active_bit);  /* DB: $B480F3D5... _particle_system_active_bit */
}
