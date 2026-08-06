/* ai_scripting_command_list_status @0x83771868 — script query: report the command-list execution status across a
 * list of unit objects, returning the maximum status found:
 *   0 = no actor / finished long ago      2 = running (current command not paused)
 *   1 = finished (or finished < 150 ticks  3 = running, paused (metadata bit 0x10 clear)
 *       ago, per the actor's finish timer)
 *
 * For each unit object (verified as type flags 3) the controlling actor is examined. A primary actor reads its
 * current command index (actor+0xA4), command-list index (actor+0x9C) and metadata flags (actor+0xA8) directly; a
 * swarm actor instead locates this unit's swarm component and reads the overlaid obey control's current command
 * index and metadata flags. The command list's command tag_block lives at command_list+0x30 (count) / +0x34
 * (address, 32-byte elements); an out-of-range or absent command means "finished" (status 1, or 2|3 while still
 * running). If the actor is not currently in the command-list action (state.action != actor_action_obey), the
 * fallback uses the actor's finish timer (actor+0x94): finished within the last 150 ticks counts as status 1. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/swarm_component_flags.h"
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/swarm_datum.h"
#include "headers/swarm_component_datum.h"
#include "headers/actor_action.h"
#include "headers/ai_command_list_definition.h"
#include "headers/object_type.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);
extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

int16_t ai_scripting_command_list_status(int object_list_index)
{
    int16_t result = 0;
    int now = game_time_get();
    int reference_index;

    for ( int unit_index = object_list_get_first(object_list_index, &reference_index);
          unit_index != -1;
          unit_index = object_list_get_next(object_list_index, &reference_index) )
    {
        unit_datum *unit_object = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
        if ( !unit_object )
            continue;

        actor_datum *actor = 0;
        int status = 0;

        if ( unit_object->unit.actor_index == -1 )   /* no primary actor — try swarm actor */
        {
            if ( unit_object->unit.swarm_actor_index == -1 )
                goto accumulate;
            actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, unit_object->unit.swarm_actor_index);
            if ( actor->state.action != actor_action_obey || actor->meta.swarm_cache_index == -1 )   /* not obeying / no swarm */
                goto accumulate;

            swarm_datum *swarm = DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
            int unit_count = swarm->unit_count;
            int slot = 0;
            while ( slot < unit_count && swarm->unit_indices[slot] != unit_index )
                slot = (int16_t)(slot + 1);
            if ( (int16_t)slot >= unit_count )
                goto accumulate;

            /* recovered: (char*)swarm_component_data->data + ((idx<<6)&0x3FFFC0) -> DATA_ARRAY_ELEMENT (stride 64 = swarm_component datum_size) */
            swarm_component_datum *component =
                DATA_ARRAY_ELEMENT(swarm_component_data, swarm_component_datum, swarm->component_indices[(int16_t)slot]);
            if ( (component->flags & (1u << _swarm_component_obey_direct_bit)) == 0 )
                goto accumulate;

            unsigned char current_command = component->___u9.obey.current_command_index;
            ai_command_list_definition_t *command_list =
                &((ai_command_list_definition_t *)global_scenario->ai_command_lists.address)[actor->state.action_data.___u0.obey.command_list_index];
            if ( current_command >= (unsigned int)command_list->commands.count
              || !(32 * current_command + (int)command_list->commands.address) )
            {
                status = 1;
                goto accumulate;
            }
            status = ((unsigned int)~component->___u9.obey.metadata_flags >> 4) & 1 | 2;
        }
        else   /* primary actor */
        {
            actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, unit_object->unit.actor_index);
            if ( actor->state.action != actor_action_obey )   /* not obeying */
                goto accumulate;

            unsigned char current_command = actor->state.action_data.___u0.obey.simple_control.current_command_index;   /* actor+0xA4 */
            ai_command_list_definition_t *command_list =
                &((ai_command_list_definition_t *)global_scenario->ai_command_lists.address)[actor->state.action_data.___u0.obey.command_list_index];
            if ( current_command >= (unsigned int)command_list->commands.count
              || !(32 * current_command + (int)command_list->commands.address) )
            {
                status = 1;
                goto accumulate;
            }
            status = ((unsigned int)~actor->state.action_data.___u0.obey.simple_control.metadata_flags >> 4) & 1 | 2;   /* actor+0xA8 metadata flags */
        }

    accumulate:
        if ( !status && actor )
        {
            int finish_time = actor->state.last_command_list_time;   /* actor+0x94 */
            if ( finish_time != -1 )
                status = finish_time + 150 >= now;
        }
        if ( status > result )
            result = (int16_t)status;   /* extsh r27 @83771AAC — the accumulator is 16-bit */
    }

    return result;
}
