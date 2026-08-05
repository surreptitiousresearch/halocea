/* prop_orphan_from_friend @0x837D1B5C — create a new orphan prop for an actor, seeded from a friend's
 * acknowledged prop. Allocates a prop datum, adds it to the actor (unitless, prop_add), and links it:
 * the unacknowledged prop's orphan_prop_index points at the new prop, and the new prop's orphan_prop_index
 * points back at the unacknowledged prop. If the friend's acknowledged prop is in engagement state 4 or 5,
 * that state is copied onto the new prop. Returns the new prop index, or -1 if allocation failed.
 *
 * DEVIATION: the prop_setup_orphan third argument is ambiguous in the disassembly — r5 is last assigned
 * v6 (the new prop) before the clobbering prop_add call, and the decompiler leaves it uninitialized; v6 is
 * used here as the most defensible static reading. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


extern int datum_new(data_array *data);
extern void prop_add(int actor_index, int unit_index, int prop_index);
extern void prop_setup_orphan(uint16_t prop_index, uint16_t orphan_prop_index, int acknowledged_prop_index);

int prop_orphan_from_friend(int actor_index, int unacknowledged_prop_index, int friend_acknowledged_prop_index)
{
    int new_prop_index = datum_new(prop_data);
    prop_add(actor_index, -1, new_prop_index);
    if (new_prop_index == -1)
        return new_prop_index;

    prop_datum *unacknowledged = DATUM_GET(prop_data, prop_datum, unacknowledged_prop_index);
    prop_datum *orphan = DATA_ARRAY_ELEMENT(prop_data, prop_datum, new_prop_index);
    prop_datum *friend = DATUM_GET(prop_data, prop_datum, friend_acknowledged_prop_index);

    prop_setup_orphan(new_prop_index, friend_acknowledged_prop_index, new_prop_index);
    unacknowledged->___u3.orphan_prop_index = new_prop_index;
    orphan->___u3.orphan_prop_index = unacknowledged_prop_index;

    int16_t friend_state = friend->state;
    if (friend_state >= _prop_state_uninspected_orphan && friend_state <= _prop_state_inspected_orphan)
        orphan->state = friend_state;

    return new_prop_index;
}
