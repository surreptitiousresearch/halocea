/* flags_dispose_from_old_map @0x8380DE60 — per-map teardown: invalidate the (CTF) flag data pool (tail-call
 * forwarder, confirmed via disasm: loads flag_data then `b data_make_invalid`). */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_invalid(data_array *data);

void flags_dispose_from_old_map(void)
{
    data_make_invalid(flag_data);
}
