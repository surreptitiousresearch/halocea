#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern void actor_perception_forget_recent_damage(int actor_index);
extern void actor_perception_retreat_successful(int actor_index);

void action_guard_begin(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    actor_perception_forget_recent_damage(actor_index);
    actor->state.searching = 0;
    if ( actor->state.action_data.___u0.guard.cower_from_retreat )
        actor_perception_retreat_successful(actor_index);
}
