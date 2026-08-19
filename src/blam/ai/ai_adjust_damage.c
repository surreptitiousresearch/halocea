/* ai_adjust_damage @0x836E78F8 — let an AI actor scale incoming damage. If the damage is weapon damage
 * (damage_data flag _damage_from_weapon_bit) and the actor has a positive burst damage modifier, apply it;
 * if the actor is playfighting, reduce damage to 30%. Returns 1 if any adjustment was made. */

#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/damage_data.h"
#include "headers/damage_data_flags.h"
#include "headers/blam_data_globals.h"


int ai_adjust_damage(int actor_index, const damage_data *damage_data, float *damage)
{
    int result = 0;
    if ( actor_index == -1 )
        return 0;

    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    /* attest: disasm reads element+0x69C = control.burst_damage_modifier (rel 0x230), not
     * burst_error (rel 0x22C / +0x698) — prior reconstruction used the wrong field. */
    if ( (damage_data->flags & (1u << _damage_from_weapon_bit)) != 0 && actor->control.burst_damage_modifier > 0.0f )
    {
        result = 1;
        *damage = actor->control.burst_damage_modifier * *damage;
    }
    if ( actor->external_orders.playfighting )
    {
        *damage = *damage * 0.30000001f;
        return 1;
    }
    return result;
}
