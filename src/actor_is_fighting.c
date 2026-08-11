/* actor_is_fighting @0x8371DBE0 — true when the actor's combat status (word +55) is at the fighting level
 * (>= 7). When additionally in the engaged sub-mode (word +54 == 4), the result is masked off if the fight
 * timer / counter (word +84) has run out.
 *
 * Deviation: reproduces the decompiler's sign/threshold mask idiom verbatim. */

#include <stdint.h>

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_action.h"
#include "headers/actor_panic_type.h"
#include "headers/actor_combat_status.h"
#include "headers/blam_data_globals.h"


/* return attested uint8_t: 4/5 callers byte-normalize r3 with clrlwi 24
 * (encounter_update_status @0x8370D764, reply_filter_*, actor_perception_update). */
uint8_t actor_is_fighting(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int status = actor->state.combat_status;
    unsigned int result = status >= _actor_combat_status_visible;
    if ( status >= _actor_combat_status_visible && actor->state.action == actor_action_flee )
        result &= ~((unsigned int)actor->state.action_data.___u0.flee.panic_type >> 31) + (actor->state.action_data.___u0.flee.panic_type != _actor_panic_none);
    return result;
}
