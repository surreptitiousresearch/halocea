/* particle_system_new_particle_default @0x8373B4F0 */
#include <stdint.h>
#include "headers/particle_system_datum.h"
#include "headers/ps_particle_datum.h"
#include "headers/object_marker.h"

void particle_system_new_particle_default(const particle_system_datum *system, int16_t type_index, ps_particle_datum *particle, object_marker *marker)
{
    particle->position = marker->matrix.position;
    particle->velocity = system->velocity;
}
