/* actor_action_handle_panic_from_attached_melee_attackers @0x837F1FF0 — if the actor has an attached
 * melee attacker, raise its panic_type to at least _actor_panic_melee_attached_to_us and force
 * panic_prop_index to -1 to trigger a panic reaction. Returns whether an attacker was attached.
 * (The floor is a panic_type priority, not a tick count — the prior "ticks" note was wrong.) */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/actor_panic_type.h"
#include "headers/blam_data_globals.h"


int actor_action_handle_panic_from_attached_melee_attackers(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if (!actor->input.melee_attacker_attached)
        return 0;

    int16_t panic_type = actor->stimuli.panic_type;
    if (panic_type <= _actor_panic_melee_attached_to_us)
        panic_type = _actor_panic_melee_attached_to_us;
    actor->stimuli.panic_type = panic_type;
    actor->stimuli.panic_prop_index = -1;
    return 1;
}
