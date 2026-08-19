/* actor_perception_unreachable @0x837D9A28 — set or clear a prop's "unreachable" state. When setting, latch
 * the first-unreachable tick stamp; when clearing, reset it. Recomputes the prop's unopposable flag and
 * target weight afterward.
 *
 * Deviation: the decompiler drops actor_compute_prop_target_weight's prop argument (r4 still holds prop_index
 * from actor_compute_prop_unopposable), so prop_index is forwarded. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);
extern uint8_t actor_compute_prop_unopposable(int actor_index, int prop_index);
extern float actor_compute_prop_target_weight(int actor_index, int prop_index);

void actor_perception_unreachable(int actor_index, int prop_index, uint8_t unreachable)
{
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
    if ( unreachable )
    {
        if ( !prop->unreachable_ticks )
            prop->unreachable_ticks = 1;
        prop->last_unreachable_time = game_time_get();
    }
    else
    {
        prop->unreachable_ticks = 0;
        prop->last_unreachable_time = -1;
    }
    prop->unopposable_enemy = actor_compute_prop_unopposable(actor_index, prop_index);
    prop->target_weight = actor_compute_prop_target_weight(actor_index, prop_index);
}
