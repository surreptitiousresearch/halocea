/* ai_communication_dispose_from_old_map @0x837CBCC8 — per-map teardown: invalidate the
 * conversation data pool. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_invalid(data_array *data);

void ai_communication_dispose_from_old_map(void)
{
    data_make_invalid(conversation_data);
}
