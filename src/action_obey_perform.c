/* action_obey_perform @0x837DF2A8 — drive one tick of the "obey" action: run the per-individual obey
 * callback over the actor's controllable units/swarm components, tracking whether they all finished
 * without early termination (obey->finished). If they did, and the obey action hasn't already been marked
 * complete, decide completion: it's immediate unless the active command list has the disable-falling-damage
 * flag (_ai_command_list_disable_falling_damage_bit, 0x10 — used here to mean "wait for landing") set and
 * the actor is currently airborne, in which case completion also requires the actor definition's "flying"
 * flag (_actor_definition_flying_bit, 0x200000) to be set. On completion,
 * records the completion time and marks the obey state finished. Returns true once the actor's current
 * action class is genuinely "obey" (11) and the obey state is marked finished. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/scenario.h"
#include "headers/global_tag_instances.h"
#include "headers/obey_state_data.h"
#include "headers/ai_command_list_definition.h"
#include "headers/ai_command_list_flags.h"
#include "headers/actor_action.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/obey_individual_simple_control.h"
#include "headers/obey_individual_complex_control.h"
#include "headers/obey_individual_iterator.h"
extern void action_obey_individuals_iterate(int actor_index, uint8_t initialize_structures, obey_state_data *state_data, obey_individual_iterator iterator, void *user_data);
extern void action_obey_individual_perform(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, uint8_t *user_data);
extern int game_time_get(void);

uint8_t action_obey_perform(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    obey_state_data *obey = &actor->state.action_data.___u0.obey;

    uint8_t all_finished = 1;
    action_obey_individuals_iterate(actor_index, 0, obey, action_obey_individual_perform, &all_finished);

    if ( all_finished && !obey->finished )
    {
        uint8_t complete = 1;

        /* stride was 24 (int-count) → real element is 96-byte ai_command_list_definition;
         * command_list[2] (byte off 8) was the folded flags load — disasm at 0x837DF356 shows
         * stride idx*96 and lwz r6,0x20(elem) = flags@0x20; rlwinm bit 27 = 0x10 = bit 4 =
         * _ai_command_list_disable_falling_damage_bit (here gates "wait for landing" behavior). */
        ai_command_list_definition_t *command_list =
            &((ai_command_list_definition_t *)global_scenario->ai_command_lists.address)[obey->command_list_index];
        if ( (command_list->flags & (1u << _ai_command_list_disable_falling_damage_bit)) != 0 && actor->input.in_midair )
        {
            /* meta.definition_index is the actor ('actr') tag; *elem reads flags@0x00. Bit 21 (0x200000)
             * is the actor definition's "flying" flag — a flyer never needs to land to finish obeying. */
            actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);
            complete = (actor_def->flags & (1u << _actor_definition_flying_bit)) != 0;
        }

        if ( complete )
        {
            actor->state.last_command_list_time = game_time_get();
            obey->finished = 1;
        }
    }

    return actor->state.action == actor_action_obey && obey->finished;
}
