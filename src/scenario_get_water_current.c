/* scenario_get_water_current @0x837C22B8 — tail-call forwarder (`ori r6, r6, 4; b scenario_get_current`;
 * confirmed via disasm, not a bare blr). ORs in flag bit 0x4 (suppress water detection while still
 * following the fog chain — see scenario_get_current.c) before forwarding; the caller-visible signature
 * is void, so scenario_get_current's own return value is discarded here exactly as it is at the real call
 * site (register r3 is simply never read by this wrapper's own callers). */

#include <stdint.h>
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/scenario_current_flags.h"

extern uint8_t scenario_get_current(const location *location, const real_point3d *position, real_vector3d *wind_vector, unsigned int flags);

void scenario_get_water_current(const location *location, const real_point3d *position,
        real_vector3d *wind_vector, unsigned int flags)
{
    scenario_get_current(location, position, wind_vector, flags | (1u << _scenario_current_force_water_bit));
}
