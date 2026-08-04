/* action_converse_perform @0x837FCB10 — drive one tick of the "converse" action: gated on
 * actor->meta.timeslice (this action only ticks on the actor's scheduled slice). Resolves and caches the
 * prop for the conversation's target unit (action_data+12) into action_data+16 the first time through.
 * Once the target prop is close enough — either within the conversation's accept radius (action_data+8)
 * for a "special" (team_index>=2) prop, or within 0.7 world units regardless — marks action_data+5 (a
 * one-shot "close enough" latch) and halts the actor for the remainder of the action. Otherwise keeps
 * walking toward the prop, marking action_data+4 ("finished") if the move fails outright. Returns
 * action_data+4 (0 = still converging, or unchanged/0 if not this actor's scheduled tick).
 *
 * The converse action state is the `.converse` arm of the state.action_data union (converse_state_data):
 * run_to_unit_index (+12) / run_to_prop_index (+16) / run_to_distance (+8) / failed (+4) / in_range (+5). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/converse_state_data.h"
#include "headers/game_team.h"
#include "headers/blam_data_globals.h"


extern int prop_get_base_by_unit_index(int actor_index, int unit_index, uint8_t acquire, uint8_t refresh_upon_acquisition);
extern uint8_t actor_move_halt(uint16_t actor_index);
extern uint8_t actor_move_to_prop(int actor_index, unsigned int prop_index, float accept_radius);

unsigned __int8 action_converse_perform(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    converse_state_data *converse = &actor->state.action_data.___u0.converse;

    if ( actor->meta.timeslice )
    {
        int *cached_prop_base_index = &converse->run_to_prop_index;
        if ( *cached_prop_base_index == -1 )
        {
            int target_unit_index = converse->run_to_unit_index;
            if ( target_unit_index != -1 )
                *cached_prop_base_index = prop_get_base_by_unit_index(actor_index, target_unit_index, 1u, 1u);
        }

        int prop_base_index = *cached_prop_base_index;
        if ( prop_base_index == -1 )
        {
            converse->failed = 1;
            return 1;
        }

        if ( !converse->in_range )
        {
            prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_base_index);
            float accept_radius = converse->run_to_distance;
            if ( (prop->team_index >= _game_team_human && prop->distance < accept_radius) || prop->distance < 0.69999999f )
                converse->in_range = 1;
        }

        if ( converse->in_range )
        {
            actor_move_halt(actor_index);
            return converse->failed;
        }

        if ( !actor_move_to_prop(actor_index, prop_base_index, converse->run_to_distance) )
            converse->failed = 1;
    }

    return converse->failed;
}
