/* actor_stimulus_prop_fleeing @0x837D5004 — stimulus handler fired when one of an actor's props (a unit
 * it is aware of) starts fleeing. If the actor's character allows it (variant flag 0x20) and its flee
 * cooldown has elapsed, the actor may join the rout — either because a friend is already fleeing
 * (actor_emotion_flee_with_friends) or by a random roll under the variant's flee chance. When it does,
 * it adopts a flee target (its own danger source, or the fleeing prop's own target if that unit is also
 * an active, prop-aware actor) and sets its emotion to "fleeing" (2). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_definition_flags2_flags.h"
#include "headers/actor_definition.h"
#include "headers/prop_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_action.h"
#include "headers/actor_panic_type.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern uint8_t actor_emotion_flee_with_friends(int actor_index, float *flee_chance);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern int prop_get_active_by_unit_index(uint16_t actor_index, int unit_index);

void actor_stimulus_prop_fleeing(uint16_t actor_index, uint16_t prop_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
    const actor_definition *definition = TAG_GET(const actor_definition, actor->meta.definition_index);

    if (prop->enemy
        || (definition->flags2 & (1u << _actor_definition_flags2_panic_in_groups_bit)) == 0
        || game_time_get() <= actor->emotions.flee_with_friends_disable_time)
        return;

    float flee_chance = definition->panic.panic_chance_friend_killed;
    if (actor_emotion_flee_with_friends(actor_index, &flee_chance)
        || real_seed_random(get_global_random_seed_address()) < flee_chance)
    {
        if (actor->stimuli.panic_type < _actor_panic_friend_same_type_killed)
        {
            int flee_target = actor->target.target_prop_index;
            if (prop->actor_index != -1)
            {
                actor_datum *fleeing_actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, prop->actor_index);
                /* +184 = state.action_data flee variant: flee_prop_index (action 4 = fleeing) */
                int fleeing_prop_index = fleeing_actor->state.action_data.___u0.flee.flee_prop_index;
                if (fleeing_actor->state.action == actor_action_flee && fleeing_prop_index != -1)
                    flee_target = prop_get_active_by_unit_index(
                        actor_index,
                        DATA_ARRAY_ELEMENT(prop_data, prop_datum, fleeing_prop_index)->unit_index);
                actor->stimuli.panic_prop_index = flee_target;
                actor->stimuli.panic_type = _actor_panic_friend_fleeing;
            }
        }
    }
}
