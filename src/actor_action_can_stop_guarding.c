/* actor_action_can_stop_guarding @0x837F2C44 — whether an actor guarding a position may stop: if it's
 * cowering (+164), compare its investigate level (+110) against cower_investigate_threshold; if it has no
 * time left on its current order (+156 <= 0), always allow stopping; otherwise allow it once the investigate
 * level clears guard_investigate_threshold, or once a secondary timer (+484) is running and it isn't
 * suppressed (+161 clear).
 *
 * Fields resolved to named actor_datum members (investigate level +110, order-time-remaining +156,
 * suppress-secondary flag +161, cowering flag +164, secondary timer +484; see ai_scripting_assess_status.c). */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_postcombat_type.h"
#include "headers/blam_data_globals.h"


unsigned __int8 actor_action_can_stop_guarding(unsigned __int16 actor_index, __int16 guard_investigate_threshold,
    __int16 cower_investigate_threshold)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->state.action_data.___u0.guard.cower )
        return actor->state.combat_status >= cower_investigate_threshold;

    if ( actor->state.action_data.___u0.guard.wait_ticks <= 0 )
        return 1;

    return actor->state.combat_status >= guard_investigate_threshold
        || (actor->external_orders.postcombat_type > actor_postcombat_none && !actor->state.action_data.___u0.guard.post_combat);
}
