/* ai_scripting_try_to_fight_player @0x83771598 — order every actor addressed by an AI index to prefer
 * fighting the players (desired_target_type 2 = players). */

#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"
#include "headers/desired_target_type.h"

extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);

void ai_scripting_try_to_fight_player(int ai_index)
{
    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);
    for ( actor_datum *actor = ai_index_actor_iterator_next(&iterator);
          actor;
          actor = ai_index_actor_iterator_next(&iterator) )
    {
        actor->external_orders.desired_target_type = _desired_target_player;
    }
}
