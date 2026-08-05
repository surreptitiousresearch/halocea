/* action_obey_command_list_setup @0x837DF000 — prepare an actor to obey a scenario command list. Validates the
 * command-list index, applies BSP gating (a command list bound to a structure BSP only runs when that BSP is the
 * current one), and for non-swarm actors initializes the obey_state_data from the command-list flags and seeds
 * every individual via action_obey_individuals_iterate(setup). Returns nonzero when the actor will obey.
 *
 * Command-list element is 96 bytes: flags dword at +0x20, structure BSP index __int16 at +0x2E. The flags decode:
 * bit0 = initiative, bit1 = (start) finished/looking seed (stored in user_data), bit2 cleared => allow_looking,
 * bit3 cleared => allow_communication. A swarm actor with no live swarm (actor+0x28 == -1) instead just records
 * the command list index at actor+0x90 when not yet running (actor+0x8 == 0). */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/ai_command_list_flags.h"
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/obey_state_data.h"
#include "headers/ai_command_list_definition.h"
#include "headers/blam_data_globals.h"

extern void *memset(void *dest, int value, unsigned int count);
extern void actor_look_secondary_stop(uint16_t actor_index);

typedef void (*obey_individual_iterator)(int actor_index, int unit_index, int16_t command_list_index,
                                         obey_individual_simple_control *simple_control,
                                         obey_individual_complex_control *complex_control, void *user_data);
extern void action_obey_individual_setup(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, uint8_t *user_data);
extern void action_obey_individuals_iterate(int actor_index, uint8_t initialize_structures, obey_state_data *state_data, obey_individual_iterator iterator, void *user_data);

uint8_t action_obey_command_list_setup(int actor_index, int16_t command_list_index, obey_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int obeying = 0;

    memset(state_data, 0, sizeof(obey_state_data));

    if ( command_list_index < 0 || command_list_index >= global_scenario->ai_command_lists.count )
        return 0;

    ai_command_list_definition *command_list =
        &((ai_command_list_definition *)global_scenario->ai_command_lists.address)[command_list_index];

    if ( actor->meta.swarm && actor->meta.swarm_cache_index == -1 )   /* swarm actor with no live swarm */
    {
        if ( !actor->meta.active )
            actor->state.command_list_index = command_list_index;   /* actor+0x90 */
    }
    else
    {
        int16_t command_list_bsp = command_list->runtime_structure_bsp_reference_index;
        if ( command_list_bsp == -1 || command_list_bsp == global_structure_bsp_index )
        {
            state_data->command_list_index = command_list_index;
            obeying = 1;
        }
    }

    if ( (unsigned char)obeying )
    {
        int command_list_flags = command_list->flags;
        unsigned int allow_looking = (~command_list_flags >> _ai_command_list_disable_looking_bit) & 1;
        unsigned char initiative = command_list_flags & (1u << _ai_command_list_allow_initiative_bit);
        unsigned char seed_finished = (command_list_flags & (1u << _ai_command_list_allow_targeting_bit)) != 0;
        unsigned int allow_communication = (~command_list_flags >> _ai_command_list_disable_communication_bit) & 1;

        if ( !allow_looking )
            actor_look_secondary_stop(actor_index);
        state_data->initiative = initiative;
        state_data->allow_looking = allow_looking;
        state_data->allow_communication = allow_communication;
        action_obey_individuals_iterate(actor_index, 1, state_data, action_obey_individual_setup, &seed_finished);
    }

    return obeying;
}
