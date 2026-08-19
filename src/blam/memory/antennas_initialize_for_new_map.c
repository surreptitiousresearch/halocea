/* antennas_initialize_for_new_map @0x8380D300 — per-map init: revalidate the antenna data pool (tail-call
 * forwarder, confirmed via disasm: loads antenna_data then `b data_make_valid`). */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);

void antennas_initialize_for_new_map(void)
{
    data_make_valid(antenna_data);
}
