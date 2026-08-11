/* prop_orphan_transition @0x837D1AB8 — split a child prop off its parent into a new "orphan" prop owned
 * by the actor, and cross-link the two props (each prop's dword +12 points at the other). Prop records
 * are stride 312.
 *
 * DEVIATION: the third argument to prop_setup_orphan (acknowledged_prop_index) is a stale register (r5)
 * the binary never reloads after the preceding prop_add call; r5 last held the new prop index, so it is
 * reproduced as such. */

#include <stdint.h>
#include "headers/prop_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern int datum_new(data_array *data);
extern void prop_add(int actor_index, int unit_index, int prop_index);
extern void prop_setup_orphan(int prop_index, int orphan_prop_index, int acknowledged_prop_index);

int prop_orphan_transition(int actor_index, int parent_prop_index)
{
    int new_prop_index = datum_new(prop_data);
    prop_add(actor_index, -1, new_prop_index);
    if ( new_prop_index != -1 )
    {
        prop_setup_orphan(new_prop_index, parent_prop_index, new_prop_index);
        DATUM_GET(prop_data, prop_datum, parent_prop_index)->___u3.orphan_prop_index = new_prop_index;
        DATUM_GET(prop_data, prop_datum, new_prop_index)->___u3.orphan_prop_index = parent_prop_index;
    }
    return new_prop_index;
}
