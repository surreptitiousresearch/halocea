/* antennas_dispose_from_old_map @0x8380D310 — per-map teardown: invalidate the antenna data pool (tail-call
 * forwarder, confirmed via disasm: loads antenna_data then `b data_make_invalid`). */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_invalid(data_array *data);

void antennas_dispose_from_old_map(void)
{
    data_make_invalid(antenna_data);
}
