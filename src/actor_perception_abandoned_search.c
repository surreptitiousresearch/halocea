/* actor_perception_abandoned_search @0x837D9BC8 — mark a search as abandoned. With no prop (NONE): clear
 * the actor's search state block (search word +964, flag bytes +956/+957, timer words +114/+116) and
 * refresh its combat status. With a prop: bump the prop's perception state word (+36) from
 * _prop_state_uninspected_orphan (4) to _prop_state_inspected_orphan (5), set its abandoned flag byte
 * (+187), and — only when that prop is the actor's current
 * target (dword +624) — refresh the target status and combat status. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


extern void actor_situation_combat_status_update(uint16_t actor_index);
extern void actor_situation_update_target_status(uint16_t actor_index);

void actor_perception_abandoned_search(int actor_index, int prop_index)
{
    if ( prop_index == -1 )
    {
        actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
        actor->firing_positions.pursuit_positions_count = 0;
        actor->firing_positions.pursuit_fired_at_orphan = 0;
        actor->firing_positions.pursuit_communicated_lost_contact = 0;
        actor->state.artificial_combat_status = _actor_combat_status_none;
        actor->state.suspicion_combat_status = _actor_combat_status_none;
        actor_situation_combat_status_update(actor_index);
        return;
    }

    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( prop->state == _prop_state_uninspected_orphan )
        prop->state = _prop_state_inspected_orphan;
    prop->abandoned_search = 1;

    if ( prop_index == actor->target.target_prop_index )
    {
        actor_situation_update_target_status(actor_index);
        actor_situation_combat_status_update(actor_index);
    }
}
