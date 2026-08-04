/* prop_orphan_update_information @0x837D1BE8 — update an orphan prop from the friend's acknowledged prop.
 * A thin forwarder to prop_setup_orphan; the actor_index parameter is unused. The acknowledged prop index
 * is passed for both the orphan's source and acknowledged slots. */

#include <stdint.h>

extern void prop_setup_orphan(uint16_t prop_index, uint16_t orphan_prop_index, int acknowledged_prop_index);

void prop_orphan_update_information(int actor_index, int orphan_prop_index, int friend_acknowledged_prop_index)
{
    prop_setup_orphan(orphan_prop_index, friend_acknowledged_prop_index, friend_acknowledged_prop_index);
}
