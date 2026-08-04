/* scenario_get_wind @0x837C22B0 — tail-call forwarder (`ori r6, r6, 8; b scenario_get_current`; confirmed
 * via disasm, not a bare blr). ORs in flag bit 0x8 (suppress water-medium reporting — see
 * scenario_get_current.c) before forwarding; the caller-visible signature is void, so
 * scenario_get_current's own return value is discarded here exactly as it is at the real call site. */

#include <stdint.h>
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern uint8_t scenario_get_current(const location *location, const real_point3d *position, real_vector3d *wind_vector, unsigned int flags);

void scenario_get_wind(const location *location, const real_point3d *position, real_vector3d *wind_vector,
        unsigned int flags)
{
    scenario_get_current(location, position, wind_vector, flags | 8);
}
