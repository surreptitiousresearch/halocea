/* actor_emotion_flee_with_friends @0x837D7F90 — scale an actor's flee chance by how its squadmates are
 * behaving. It scans the actor's props for living, same-species allies: an ally already fleeing (or about
 * to retreat) counts toward "fleeing friends", while an ally still fighting counts toward "combat friends".
 * Two or more fleeing friends forces a flee (returns 1). Otherwise the flee chance is boosted when friends
 * are also withdrawing and damped when friends are holding the line, clamped to a 0..2x multiplier.
 *
 * The allied actor's fields are read by database byte offset (no reversed actor struct). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_iterator.h"
#include "headers/prop_datum.h"
#include "headers/actor_action.h"
#include "headers/actor_panic_type.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);

uint8_t actor_emotion_flee_with_friends(int actor_index, float *flee_chance)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    __int16 fleeing_friends = 0;
    __int16 combat_friends = 0;

    prop_iterator iterator;
    prop_iterator_new(&iterator, actor_index);
    for (prop_datum *prop = prop_iterator_next(&iterator); prop; prop = prop_iterator_next(&iterator))
    {
        int state = prop->state;
        if (state >= _prop_state_becoming_unacknowledged && state <= _prop_state_acknowledged && !prop->enemy
            && (unsigned __int16)prop->type == (unsigned __int16)actor->meta.type
            && prop->actor_index != -1)
        {
            actor_datum *friend = DATA_ARRAY_ELEMENT(actor_data, actor_datum, prop->actor_index);
            char *friend_bytes = (char *)friend;
            if (friend->stimuli.panic_type > _actor_panic_none || (friend->state.action == actor_action_flee && friend->state.action_data.___u0.flee.panic_type > _actor_panic_none))
                ++fleeing_friends;
            else if (prop->in_combat)
                ++combat_friends;
        }
    }

    if (fleeing_friends > 1)
        return 1;

    float multiplier;
    if (combat_friends <= 1)
    {
        int withdrawing = 1 - combat_friends;
        multiplier = (((float)withdrawing * 0.5f) + 1.0f);
    }
    else
    {
        int holding = combat_friends - 1;
        multiplier = (-(((float)holding * 0.25f) - 1.0f));
    }

    float clamp = 0.0f;
    if (multiplier < 0.0f || (clamp = 2.0f, multiplier > 2.0f))
        multiplier = clamp;
    *flee_chance = multiplier * *flee_chance;
    return 0;
}
