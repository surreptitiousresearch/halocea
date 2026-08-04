/* prop_delete @0x837D1EE0 — unlink a prop from its actor and free the datum.
 *
 * Deviation: the decompiler drops datum_delete's second argument (r4 still holds prop_index from the
 * prop_remove call), so prop_index is forwarded here. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void prop_remove(uint16_t actor_index, int prop_index);
extern void datum_delete(data_array *data, int index);

void prop_delete(int actor_index, int prop_index)
{
    prop_remove(actor_index, prop_index);
    datum_delete(prop_data, prop_index);
}
