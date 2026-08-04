/* find_netgame_flag @0x83747670 — singular wrapper around find_netgame_flags: the "find one" form.
 * DEVIATION (2026-07-31): five params, not seven. disasm shows the max (r8=1) and flags (r9=&local)
 * arguments to find_netgame_flags are set INTERNALLY, not received from callers — the earlier 6th/7th
 * params (`max`, `flags`) were phantoms. A single-int local is pre-seeded to -1, passed as the output
 * array (capacity 1), and its post-call value is returned: the index of the first matching flag, or -1.
 * (The earlier "unconditionally returns -1" reading missed the `lwz r3, local` at 0x83747690.) */

#include <stdint.h>
#include "headers/real_point3d.h"

extern int find_netgame_flags(const real_point3d *location, float distance, float height_delta, int16_t type, int16_t team, int max, int *flags);

int find_netgame_flag(real_point3d *location, float distance, float height_delta, int16_t type, int16_t team)
{
    int found_flag = -1;
    find_netgame_flags(location, distance, height_delta, type, team, 1, &found_flag);
    return found_flag;
}
