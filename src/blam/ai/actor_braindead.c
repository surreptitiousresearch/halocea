/* actor_braindead @0x837208C0 — make an actor brain-dead (limp/disabled) or revive it. When braindead is set,
 * the actor's active/desired states (actor+0x6A, actor+0x6C) are zeroed, its props are released, its unit is
 * driven to the frozen pose, and it is taken out of dormancy. Clearing braindead (state @actor+0x6A == 0) restores
 * the actor to the default active state (2). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_action.h"
#include "headers/actor_mode.h"
#include "headers/blam_data_globals.h"

extern void actor_delete_props(int actor_index);
extern void actor_freeze(int actor_index);
extern void actor_set_dormant(int actor_index, uint8_t dormant);

void actor_braindead(int actor_index, uint8_t braindead)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( braindead )
    {
        actor->state.mode = _actor_mode_braindead;   /* actor+0x6A — current state */
        actor->state.action = actor_action_none;   /* actor+0x6C — desired state */
        actor_delete_props(actor_index);
        actor_freeze(actor_index);
        actor_set_dormant(actor_index, 0);
    }
    else if ( !actor->state.mode )
    {
        actor->state.mode = _actor_mode_alert;   /* revive to default active state */
    }
}
