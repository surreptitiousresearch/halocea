/* new_particle_is_visible @0x8373DB58 — visibility check for a newly-spawned particle, delegating entirely
 * to scenario_location_potentially_visible_local.
 *
 * DEVIATION: `location`/`position`/`radius` are dead — disasm_range (0x8373DB58-0x8373DB7C) shows the call
 * touches only r3 unmodified from entry (no register setup at all before `bl`), and
 * scenario_location_potentially_visible_local's own DB prototype takes a single `const location *`. So
 * `definition` (r3, this function's real first argument) is actually a location pointer at the ABI level
 * despite the DB's `particle_definition *` name for it; reinterpreted here to match the real call. */

#include <stdint.h>
#include "headers/particle_definition.h"
#include "headers/location.h"
#include "headers/real_point3d.h"

extern uint8_t scenario_location_potentially_visible_local(const location *location);

uint8_t new_particle_is_visible(const particle_definition *definition, const location *location, const real_point3d *position, float radius)
{
    return scenario_location_potentially_visible_local((const struct location *)definition);
}
