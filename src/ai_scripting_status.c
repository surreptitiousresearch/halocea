/* ai_scripting_status @0x83771F50 — return the highest combat-escalation status among the actors addressed
 * by an AI index. Per actor: 0 = inactive, 1 = active/idle, 2 = alert, 3 = searching, 4 = engaging,
 * 5 = in close combat, 6 = in close combat and either firing or has taken damage. The maximum over all
 * addressed actors is returned. */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"
#include "headers/actor_action.h"
#include "headers/actor_target_type.h"
#include "headers/blam_data_globals.h"

extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);

int ai_scripting_status(int ai_index)
{
    int max_status = 0;
    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);
    while ( ai_index_actor_iterator_next(&iterator) )
    {
        actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, iterator.iterator.index);
        __int16 status;
        if ( actor->meta.active )                              /* meta.active */
        {
            if ( actor->state.action >= actor_action_fight )                    /* actor +0x6A combat phase */
            {
                if ( actor->state.combat_status )              /* actor +0x6E has-enemy */
                {
                    int combat_level = actor->target.target_type;   /* actor +0x268 */
                    if ( combat_level >= actor_target_definite_orphan )
                    {
                        if ( combat_level >= actor_target_visible_enemy )
                            status = (actor->orders.combat.shoot_at_target || actor->orders.combat.throw_grenade != 0) ? 6 : 5;
                        else
                            status = 4;
                    }
                    else
                    {
                        status = 3;
                    }
                }
                else
                {
                    status = 2;
                }
            }
            else
            {
                status = 1;
            }
        }
        else
        {
            status = 0;
        }
        if ( (__int16)max_status > status )
            status = max_status;
        max_status = status;
    }
    return max_status;
}
