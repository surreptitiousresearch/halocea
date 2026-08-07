/* actor_emotion_assess_unopposable_danger @0x837D81D8 — classifies how dangerous a "no longer opposable"
 * enemy prop is (actor_unopposable_danger_type), purely from the prop's own state: none if it's not a
 * recently-lost enemy (state outside [2,3]) or not flagged unopposable; damaging if it's currently damaging
 * the actor; if shooting, shooting (facing away, quantized_facing<=1) or shooting_nearby (facing us);
 * otherwise visible if still visible-ish (visibility>=2), else none.
 *
 * DEVIATION: the FIRST parameter is the prop_data subscript, not the second. `clrlwi r11, r3, 16`
 * @0x837D81DC feeds `mulli r10, r11, 0x138` (0x138 = sizeof(prop_datum)); r4 is never read at all.
 * This file previously claimed the opposite and cited disasm_range(0x837D81E4, 0x837D8270) as proof —
 * but 0x837D81E4 is INTERIOR to the function, which starts at 0x837D81D8, so that range began one
 * instruction AFTER the `clrlwi` that settles it. The evidence had been truncated by a wrong start
 * address, and the conclusion inverted. Found by tools/paramdrop.py (C4100 vs the prologue).
 *
 * CAVEAT: the DB prototype names the parameters (actor_index, prop_index), which disagrees with the
 * register evidence about which one indexes prop_data. The function has NO callers in the image, so
 * nothing adjudicates the naming; the names are kept verbatim from the DB and the subscript follows
 * the register. Do not "fix" the apparent mismatch by renaming without caller evidence. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_unopposable_danger_type.h"
#include "headers/blam_data_globals.h"


int16_t actor_emotion_assess_unopposable_danger(int actor_index, int prop_index)
{
    /* clrlwi r11, r3, 16 clears the LEFT 16 bits: the subscript is the low half zero-extended. */
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, (uint16_t)actor_index);

    if ( prop->state < _prop_state_becoming_unacknowledged || prop->state > _prop_state_acknowledged || !prop->unopposable_enemy )
        return _actor_unopposable_danger_none;

    if ( prop->currently_damaging_me )
        return _actor_unopposable_danger_damaging;

    if ( prop->shooting )
        return prop->quantized_facing > 1 ? _actor_unopposable_danger_shooting : _actor_unopposable_danger_shooting_nearby;

    return prop->visibility >= 2 ? _actor_unopposable_danger_visible : _actor_unopposable_danger_none;
}
