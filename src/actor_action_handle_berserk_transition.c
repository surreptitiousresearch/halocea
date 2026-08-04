/* actor_action_handle_berserk_transition @0x837F68A0 — if the actor's berserk timer (word +392) has reached
 * `berserk_level` and it isn't already berserking (byte +888, see actor_berserk.c), enters berserk and, if
 * awareness (word +55, see action_fight_control.c) is at least 4, re-evaluates its combat action. Always
 * clears the berserk timer afterward. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_berserk_type.h"
#include "headers/actor_combat_status.h"
#include "headers/blam_data_globals.h"

extern void actor_berserk(uint16_t actor_index, uint8_t berserk);
extern int actor_action_handle_combat_selection(int actor_index);

int actor_action_handle_berserk_transition(int actor_index, int16_t berserk_level)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int result = 0;

    if ( actor->stimuli.berserk_type >= berserk_level && !actor->emotions.berserk )
    {
        actor_berserk(actor_index, 1u);

        /* recovered: disasm at 0x837F68F4 reads 0x6E (combat_status), not 0x6A (mode); cmpwi 4.
         * Prior render used state.mode >= NUMBER_OF_ACTOR_MODES (wrong field; NUMBER_OF_ACTOR_MODES==4 was coincidental). */
        if ( actor->state.combat_status >= _actor_combat_status_certain )
            result = actor_action_handle_combat_selection(actor_index);
    }

    actor->stimuli.berserk_type = actor_berserk_none;
    return result;
}
