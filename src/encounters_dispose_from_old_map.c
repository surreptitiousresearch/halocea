/* encounters_dispose_from_old_map @0x83709184 — per-map teardown: invalidate the encounter and pursuit
 * data pools. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_invalid(data_array *data);

void encounters_dispose_from_old_map(void)
{
    data_make_invalid(encounter_data);
    data_make_invalid(pursuit_data);
}
