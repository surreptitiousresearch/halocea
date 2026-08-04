/* action_guard_replace_prop @0x83824778 — guard-action prop substitution: when the actor's discarded-
 * prop slot (offset +216) or guard-target prop slot (offset +156, dword at +16 within it) references
 * invalid_prop_index, replace it with replacement_prop_index. If the guard-target slot is cleared to -1,
 * also clear its associated flag byte at +171. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void action_guard_replace_prop(uint16_t actor_index, int invalid_prop_index, int replacement_prop_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int *discarded_prop_index = &actor->state.action_data.___u0.guard.guard_look_prop_index;
    int *guard_target_prop_index = &actor->state.action_data.___u0.guard.shout_dead_friend_prop_index;

    if (*discarded_prop_index == invalid_prop_index)
        *discarded_prop_index = replacement_prop_index;

    if (*guard_target_prop_index == invalid_prop_index)
    {
        *guard_target_prop_index = replacement_prop_index;
        if (replacement_prop_index == -1)
            actor->state.action_data.___u0.guard.shout_about_dead_friend = 0;
    }
}
