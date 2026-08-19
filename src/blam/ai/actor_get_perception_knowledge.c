/* actor_get_perception_knowledge @0x837D5480 — classify how much "combat knowledge" the actor has about a
 * prop's target (0..3), used to scale visibility tests. Acknowledged/searching props, audible-class props and
 * live non-enemies grade as full knowledge (3); an orphaned prop inherits from its orphan; otherwise the
 * answer falls back to the actor's own awareness level.
 *
 * DEVIATION: actor_index is `int`, per the DB (`__int16 actor_get_perception_knowledge(int actor_index,
 * int prop_index)`) and value-neutral in the body — `clrlwi r11, r3, 16` @0x837D5484 is r3's ONLY use as
 * the incoming argument (`li r3, -1` @0x837D5488 redefines the register on the very next instruction), so
 * the whole word never leaves this function. The uint16_t spelling it carried was the sibling-convention
 * rule UR-37 / #112b(C) refuted as a class; the low-word extract the clrlwi performs stays explicit as
 * DATA_ARRAY_ELEMENT's own `(uint16_t)(index)` — zero-extending, which is what a clrlwi site requires. */

#include "headers/prop_datum.h"
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_knowledge.h"
#include "headers/ai_unit_effect.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


int16_t actor_get_perception_knowledge(int actor_index, int prop_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int result = -1;

    if ( prop_index != -1 )
    {
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
        int16_t state = prop->state;
        int sound_class = prop->unit_effect;
        if ( (state >= _prop_state_becoming_unacknowledged && state <= _prop_state_acknowledged)
          || sound_class == _ai_unit_effect_shooting
          || sound_class == _ai_unit_effect_death_scream
          || (!prop->enemy && (!prop->dead || actor->state.mode >= _actor_mode_combat)) )
        {
            result = _actor_knowledge_definite;
        }
        if ( (int16_t)result == -1 && prop->___u3.orphan_prop_index != -1 )
            /* recovered: prop_pool[312*idx + 184] -> prop_datum.definitely_located (312 = sizeof(prop_datum)).
             * Latch -> _actor_knowledge_definite when set, else _actor_knowledge_searching. */
            result = (DATA_ARRAY_ELEMENT(prop_data, prop_datum,
                                         prop->___u3.orphan_prop_index)->definitely_located != 0)
                     + _actor_knowledge_searching;
    }

    if ( (int16_t)result == -1 )
    {
        if ( actor->state.combat_status < _actor_combat_status_investigate )
            /* sign/compare trick yields _actor_knowledge_noncombat (0) or _actor_knowledge_guard (1) */
            return (actor->state.mode >> 31) + ((unsigned int)actor->state.mode >= _actor_mode_combat);
        return _actor_knowledge_searching;
    }
    return result;
}
