/* action_obey_individuals_iterate @0x837DEEB0 — drive the per-individual command-list callback over an actor's
 * controllable individuals. For a single-unit actor (actor+0x6 == 0) the callback runs once with the actor's unit
 * and the shared obey_state_data simple/complex controls. For a swarm actor it runs once per live swarm component:
 * when initialize_structures is set, each component's overlaid obey control is zeroed and its flags are reset to
 * the "active" sub-state (flags = (flags & ~0xC) | 8); only components whose flag 0x08 is set are visited, and the
 * callback receives that component's overlaid simple control (no complex control for swarm individuals).
 *
 * DEVIATION: the decompiler renders the per-component zero-init as a 9-dword unrolled loop over swarm_component+28
 * (the obey/wander union, 36 bytes); reproduced as a memset of that union. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/swarm_component_flags.h"
#include "headers/data_array.h"
#include "headers/swarm_datum.h"
#include "headers/swarm_component_datum.h"
#include "headers/obey_state_data.h"
#include "headers/blam_data_globals.h"

extern void *memset(void *dest, int value, unsigned int count);

typedef void (*obey_individual_iterator)(int actor_index, int unit_index, int16_t command_list_index,
                                         obey_individual_simple_control *simple_control,
                                         obey_individual_complex_control *complex_control, void *user_data);

void action_obey_individuals_iterate(int actor_index, uint8_t initialize_structures, obey_state_data *state_data, obey_individual_iterator iterator, void *user_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( !actor->meta.swarm )   /* single-unit actor */
    {
        iterator(actor_index, actor->meta.unit_index, state_data->command_list_index,
                 &state_data->simple_control, &state_data->complex_control, user_data);
        return;
    }

    swarm_datum *swarm = DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
    for ( int i = 0; i < swarm->unit_count; i = (int16_t)(i + 1) )
    {
        /* recovered: (char*)swarm_component_data->data + ((idx<<6)&0x3FFFC0) -> DATA_ARRAY_ELEMENT (stride 64 = swarm_component datum_size) */
        swarm_component_datum *component =
            DATA_ARRAY_ELEMENT(swarm_component_data, swarm_component_datum, swarm->component_indices[i]);
        if ( initialize_structures )
        {
            memset(&component->___u9.obey, 0, sizeof(component->___u9.obey));
            /* ~0xC clears wander_valid + obey_direct; |8 re-sets obey_direct. */
            component->flags = (component->flags
                    & ~((1u << _swarm_component_wander_valid_bit) | (1u << _swarm_component_obey_direct_bit)))
                | (1u << _swarm_component_obey_direct_bit);
        }
        if ( (component->flags & (1u << _swarm_component_obey_direct_bit)) != 0 )
            iterator(actor_index, swarm->unit_indices[i], state_data->command_list_index,
                     &component->___u9.obey, 0, user_data);
    }
}
