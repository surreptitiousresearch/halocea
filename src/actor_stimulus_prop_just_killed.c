/* actor_stimulus_prop_just_killed @0x837D4E2C — react to a perception prop being killed. Flags the kill on
 * the actor, then (only when the killed prop was a friend) decides how the actor responds based on its
 * actor definition: it may switch its glance/emotion toward the killer prop with a definition-driven
 * probability, and — if the killer is an enemy prop that is sufficiently fresh/close — it may flee with
 * friends or raise its fear emotion, and bumps the killer prop's "killed a friend" bookkeeping.
 *
 * actor_data has a 1828-byte stride, prop_data 312 bytes. The actor definition is fetched via the actor's
 * variant tag instance. Field offsets are kept raw, matching the engine's datum layouts. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags2_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_panic_type.h"
#include "headers/blam_data_globals.h"


extern int actor_perception_find_killer_prop_index(int actor_index, uint16_t killed_prop_index, uint8_t skip_friendlies);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern int game_time_get(void);
extern uint8_t actor_emotion_flee_with_friends(int actor_index, float *flee_chance);

void actor_stimulus_prop_just_killed(uint16_t actor_index, uint16_t prop_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

    actor->state.been_in_combat = 1; /* perceived a kill this tick */

    if (prop->enemy) /* killed prop was an enemy — nothing to mourn */
        return;

    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);
    int killer_prop_index = actor_perception_find_killer_prop_index(actor_index, prop_index, 1u);

    /* if the killed friend matches the actor definition's "notable ally" unit and the actor's current
     * emotion priority leaves room, maybe latch a strong reaction toward the killer */
    if ((unsigned __int16)prop->type == (unsigned __int16)character->panic.panic_leader_type
        && actor->stimuli.panic_type < _actor_panic_friend_leader_type_killed)
    {
        if (real_seed_random(get_global_random_seed_address()) < (double)character->panic.panic_chance_leader_type_killed)
        {
            actor->stimuli.panic_prop_index = killer_prop_index;
            actor->stimuli.panic_type = _actor_panic_friend_leader_type_killed;
        }
    }

    if (prop->distance < 8.0 && killer_prop_index != -1)
    {
        prop_datum *killer_prop = DATUM_GET(prop_data, prop_datum, killer_prop_index);
        if (killer_prop->enemy)
        {
            if (killer_prop->visibility > 0 && killer_prop->quantized_facing <= 2)
            {
                int character_flags2 = character->flags2;
                float flee_chance = character->panic.panic_chance_friend_killed;
                unsigned __int8 fled = 0;
                if ((character_flags2 & (1u << _actor_definition_flags2_panic_in_groups_bit)) != 0 && game_time_get() > actor->emotions.flee_with_friends_disable_time)
                    fled = actor_emotion_flee_with_friends(actor_index, &flee_chance);

                if (fled || real_seed_random(get_global_random_seed_address()) < flee_chance)
                {
                    if (actor->stimuli.panic_type < _actor_panic_friend_same_type_killed)
                    {
                        actor->stimuli.panic_prop_index = killer_prop_index;
                        actor->stimuli.panic_type = _actor_panic_friend_same_type_killed;
                    }
                }
            }

            if (killer_prop->unopposable_enemy)
            {
                __int16 kills = killer_prop->unopposable_casualties_inflicted;
                killer_prop->unopposable_casualty_decay_timer = 750;
                killer_prop->unopposable_casualties_inflicted = kills + 1;
            }
        }
    }
}
