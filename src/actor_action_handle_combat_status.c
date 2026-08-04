/* actor_action_handle_combat_status @0x837F6508 — decide, from an actor's current action class and combat
 * status, whether it should re-select a combat action or fall back to "lost contact" behaviour. The action's
 * class (from the global action dispatch table) gates the policy: classes 1/2 only act on their own initiative;
 * class 3 additionally consults the actor's pursued prop; class 4 always resolves. `force_decision` forces a
 * decision (implying allow_initiative) and, if nothing else fired, defaults to lost-contact. Returns the handler
 * result (non-zero low byte = handled).
 *
 * DEVIATION: the action sub-mode (+164) resolves to the search arm's pursuit_location.type
 * (action_data+8); prop fields use the DB prop_datum struct. The goto-based dispatch is reflowed into nested conditionals. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/action_specification.h"
#include "headers/actor_action.h"
#include "headers/action_class.h"
#include "headers/actor_postcombat_type.h"
#include "headers/actor_combat_status.h"
#include "headers/blam_data_globals.h"


extern int actor_action_handle_combat_selection(int actor_index);
extern uint8_t actor_action_handle_lost_contact(int actor_index);

uint8_t actor_action_handle_combat_status(int actor_index, uint8_t allow_initiative, uint8_t force_decision)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int result = 0;

    if ( force_decision )
        allow_initiative = 1;

    __int16 action_class = global_action_functions[actor->state.action].action_class;
    if ( (unsigned int)(action_class - _action_class_passive) <= 3 )   /* action_class in [passive, active] */
    {
        if ( action_class == _action_class_passive || action_class == _action_class_transitory )
        {
            if ( allow_initiative )
            {
                int combat_status = actor->state.combat_status;
                if ( combat_status >= _actor_combat_status_clear_los )
                {
                    result = actor_action_handle_combat_selection(actor_index);
                }
                else if ( combat_status >= _actor_combat_status_investigate || actor->state.action == actor_action_alert
                       || (combat_status == _actor_combat_status_none
                           && (actor->external_orders.stand_down || actor->external_orders.postcombat_type > actor_postcombat_none)) )
                {
                    result = actor_action_handle_lost_contact(actor_index);
                }
            }
        }
        else if ( action_class == _action_class_pursuit )
        {
            if ( allow_initiative && actor->state.combat_status >= _actor_combat_status_certain )
            {
                result = actor_action_handle_combat_selection(actor_index);
            }
            else if ( actor->state.combat_status < _actor_combat_status_investigate )
            {
                result = actor_action_handle_lost_contact(actor_index);
            }
            else
            {
                int prop_index = actor->target.target_prop_index;
                if ( prop_index != -1 )
                {
                    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
                    __int16 action = actor->state.action;
                    /* uncover (action 5) shares the identical pursuit_location at this offset */
                    unsigned short action_submode = actor->state.action_data.___u0.search.pursuit_location.type;
                    char lost = 0;
                    if ( prop_index != actor->firing_positions.pursuit_prop_index
                      || (!prop->tried_to_uncover && (action != actor_action_uncover || action_submode))
                      || (!prop->tried_to_search && (action != actor_action_uncover || action_submode) && (action != actor_action_search || action_submode)) )
                        lost = 1;
                    if ( lost )
                        result = actor_action_handle_lost_contact(actor_index);
                }
            }
        }
        else   /* _action_class_active */
        {
            if ( actor->state.combat_status < _actor_combat_status_certain )
                result = actor_action_handle_lost_contact(actor_index);
            else
                result = actor_action_handle_combat_selection(actor_index);
        }
    }

    if ( !(unsigned char)result && force_decision )
        return actor_action_handle_lost_contact(actor_index);
    return result;
}
