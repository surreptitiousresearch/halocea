/* prop_new_unacknowledged @0x837D1CC8 — allocate a prop slot for a newly noticed unit. It scans the actor's
 * existing props for the best one to evict, keeping TWO candidates: the nearest prop the actor no longer
 * desires (actor_perception_desire_prop returns false), and, as a fallback, the nearest still-desired prop
 * on the same side as `enemy` for which desire_prop raised its secondary "interested but not desiring"
 * output. The fallback is only taken when the actor already tracks at least 6 same-side props when `enemy`
 * is set, or 4 when it is clear. If a candidate is found it is switched out, removed and cleared (preserving
 * its datum identifier) for reuse; otherwise a fresh prop datum is allocated. Either way prop_add binds it
 * to the unit.
 *
 * DEVIATION: the fallback track was missing entirely — the pseudocode dropped the `enemy` parameter, the
 * same-side tally and the second candidate, keeping only the tally's increment. Recovered from
 * 0x837D1DDC-0x837D1E58: `lbz r11, 0x60(r31)` / `clrlwi r10, r24, 24` / `cmplw` is prop->enemy == enemy,
 * and the threshold is `addi r7, r11, 4` over a bit lifted from `enemy != 0`, i.e. 4 or 6.
 *
 * Deviation: actor_perception_desire_prop's two float args (suicide_radius, distance_squared) make its
 * trailing int args float-GPR-skip phantoms in the decompiler; from disasm they are dead_ticks (prop+0x76),
 * required_ticks (prop+0x6A) and an optional output reference (a zeroed local byte). The decompiler's
 * matching-enemy counter is computed but unused. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/actor_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"

extern uint8_t actor_perception_desire_prop(uint16_t actor_index, int16_t desired_target_state, int unit_index, int unit_actor_index, uint8_t in_use, uint8_t player, uint8_t enemy, uint8_t dead, int16_t dead_ticks, float suicide_radius, float distance_squared, int16_t required_ticks, uint8_t *optional_reference);
extern int datum_new(data_array *data);
extern void actor_switch_props(uint16_t actor_index, int prop_index, int replacement_prop_index);
extern void prop_remove(uint16_t actor_index, int prop_index);
extern void prop_add(int actor_index, int unit_index, int prop_index);

int prop_new_unacknowledged(int actor_index, int unit_index, uint8_t enemy)
{
    float best_undesired_distance = 3.4028235e38f;
    float best_same_side_distance = 3.4028235e38f;
    int eviction_prop_index = -1;
    int same_side_prop_index = -1;
    int16_t same_side_count = 0;
    /* was raw *((int*)actor_data->data + 457*actor_index + 20): actor_datum.meta.first_prop_index @0x50 */
    int prop_index = DATUM_GET(actor_data, actor_datum, actor_index)->meta.first_prop_index;

    while ( prop_index != -1 )
    {
        int current_prop_index = prop_index;
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
        int16_t state = prop->state;
        prop_index = prop->next_prop_index;

        if ( (state < _prop_state_uninspected_orphan || state > _prop_state_inspected_orphan) && prop->___u3.orphan_prop_index == -1 )
        {
            uint8_t secondary_interest = 0;
            if ( actor_perception_desire_prop(actor_index, -1, prop->unit_index, prop->actor_index,
                                              prop->in_use, prop->player, prop->enemy, prop->dead,
                                              prop->dead_ticks, prop->suicide_radius,
                                              prop->distance * prop->distance, prop->required_ticks,
                                              &secondary_interest) )
            {
                if ( prop->enemy == enemy )
                {
                    same_side_count = (int16_t)(same_side_count + 1);
                    if ( secondary_interest && prop->distance < best_same_side_distance )
                    {
                        same_side_prop_index = current_prop_index;
                        best_same_side_distance = prop->distance;
                    }
                }
            }
            else if ( prop->distance < best_undesired_distance )
            {
                eviction_prop_index = current_prop_index;
                best_undesired_distance = prop->distance;
            }
        }
    }

    if ( eviction_prop_index == -1 && same_side_prop_index != -1
        && same_side_count >= (enemy ? 6 : 4) )
        eviction_prop_index = same_side_prop_index;

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
        int16_t identifier = prop->identifier;   /* preserve datum salt header across the wipe */
        memset(prop, 0, sizeof(prop_datum));
        prop->identifier = identifier;
    }

    prop_add(actor_index, unit_index, new_prop_index);
    return new_prop_index;
}
