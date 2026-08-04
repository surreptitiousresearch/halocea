/* prop_new_unacknowledged @0x837D1CC8 — allocate a prop slot for a newly noticed unit. It first scans the
 * actor's existing props for the best one to evict: any prop the actor no longer desires
 * (actor_perception_desire_prop returns false) and that is the farthest away (largest distance) becomes the
 * eviction candidate. If one is found it is switched out, removed and cleared (preserving its datum
 * identifier) for reuse; otherwise a fresh prop datum is allocated. Either way prop_add binds it to the unit.
 *
 * Deviation: actor_perception_desire_prop's two float args (suicide_radius, distance_squared) make its
 * trailing int args float-GPR-skip phantoms in the decompiler; from disasm they are dead_ticks (prop+0x76),
 * required_ticks (prop+0x6A) and an optional output reference (a zeroed local byte). The decompiler's
 * matching-enemy counter is computed but unused. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/actor_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"
extern void *memset(void *dst, int value, unsigned int n);

extern uint8_t actor_perception_desire_prop(uint16_t actor_index, int16_t desired_target_state, int unit_index, int unit_actor_index, uint8_t in_use, uint8_t player, uint8_t enemy, uint8_t dead, int16_t dead_ticks, float suicide_radius, float distance_squared, int16_t required_ticks, uint8_t *optional_reference);
extern int datum_new(data_array *data);
extern void actor_switch_props(uint16_t actor_index, int prop_index, int replacement_prop_index);
extern void prop_remove(uint16_t actor_index, int prop_index);
extern void prop_add(int actor_index, int unit_index, int prop_index);

int prop_new_unacknowledged(int actor_index, int unit_index, uint8_t enemy)
{
    float best_distance = 3.4028235e38f;
    int eviction_prop_index = -1;
    /* was raw *((int*)actor_data->data + 457*actor_index + 20): actor_datum.meta.first_prop_index @0x50 */
    int prop_index = DATUM_GET(actor_data, actor_datum, actor_index)->meta.first_prop_index;

    while ( prop_index != -1 )
    {
        int current_prop_index = prop_index;
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
        __int16 state = prop->state;
        prop_index = prop->next_prop_index;

        if ( (state < _prop_state_uninspected_orphan || state > _prop_state_inspected_orphan) && prop->___u3.orphan_prop_index == -1 )
        {
            unsigned __int8 desire_scratch = 0;
            if ( actor_perception_desire_prop(actor_index, -1, prop->unit_index, prop->actor_index,
                                              prop->in_use, prop->player, prop->enemy, prop->dead,
                                              prop->dead_ticks, prop->suicide_radius,
                                              prop->distance * prop->distance, prop->required_ticks,
                                              &desire_scratch) )
            {
                /* desired prop kept; matching-enemy tally is computed but unused */
            }
            else if ( prop->distance < best_distance )
            {
                eviction_prop_index = current_prop_index;
                best_distance = prop->distance;
            }
        }
    }

    int new_prop_index;
    if ( eviction_prop_index == -1 )
    {
        new_prop_index = datum_new(prop_data);
    }
    else
    {
        new_prop_index = eviction_prop_index;
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, eviction_prop_index);
        actor_switch_props(actor_index, eviction_prop_index, -1);
        prop_remove(actor_index, eviction_prop_index);
        __int16 identifier = prop->identifier;   /* preserve datum salt header across the wipe */
        memset(prop, 0, sizeof(prop_datum));
        prop->identifier = identifier;
    }

    prop_add(actor_index, unit_index, new_prop_index);
    return new_prop_index;
}
