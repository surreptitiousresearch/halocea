/* devices_dispose_from_old_map @0x837B4A48 — per-map teardown: invalidate the device group data pool
 * (tail-call forwarder, confirmed via disasm: loads device_groups_data then `b data_make_invalid`). */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_invalid(data_array *data);

void devices_dispose_from_old_map(void)
{
    data_make_invalid(device_groups_data);
}
