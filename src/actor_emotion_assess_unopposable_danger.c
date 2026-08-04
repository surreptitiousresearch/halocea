/* actor_emotion_assess_unopposable_danger @0x837D81E4 — classifies how dangerous a "no longer opposable"
 * enemy prop is (actor_unopposable_danger_type), purely from the prop's own state: none if it's not a
 * recently-lost enemy (state outside [2,3]) or not flagged unopposable; damaging if it's currently damaging
 * the actor; if shooting, shooting (facing away, quantized_facing<=1) or shooting_nearby (facing us);
 * otherwise visible if still visible-ish (visibility>=2), else none.
 *
 * DEVIATION: the decompiler misattributes the prop_data index to `actor_index` (`312 * actor_index`);
 * disasm_range(0x837D81E4,0x837D8270) confirms the multiplied register is really `prop_index` (the 2nd
 * parameter) — `actor_index` is never read anywhere in this function. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_unopposable_danger_type.h"
#include "headers/blam_data_globals.h"


int16_t actor_emotion_assess_unopposable_danger(int actor_index, int prop_index)
{
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

    if ( prop->state < _prop_state_becoming_unacknowledged || prop->state > _prop_state_acknowledged || !prop->unopposable_enemy )
        return _actor_unopposable_danger_none;

    if ( prop->currently_damaging_me )
        return _actor_unopposable_danger_damaging;

    if ( prop->shooting )
        return prop->quantized_facing > 1 ? _actor_unopposable_danger_shooting : _actor_unopposable_danger_shooting_nearby;

    return prop->visibility >= 2 ? _actor_unopposable_danger_visible : _actor_unopposable_danger_none;
}
