/* ai_scripting_assess_status @0x83771EA8 — report an actor's scripting-relevant status as a small enum: 0 not
 * under script control (meta.active clear), 1 not yet past the initial action-state threshold (state.mode < _actor_mode_combat),
 * 2 no active order (state.combat_status == _actor_combat_status_none), 3/4 progressing through order milestones (target.target_type < actor_target_definite_orphan
 * / < 10), 6 order complete on either of two completion flags (orders.combat.shoot_at_target or
 * orders.combat.throw_grenade), else 5 (in progress). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/actor_target_type.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/blam_data_globals.h"


int16_t ai_scripting_assess_status(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( !actor->meta.active )
        return 0;
    if ( actor->state.mode < _actor_mode_combat )
        return 1;
    if ( !actor->state.combat_status )
        return 2;

    int16_t progress = actor->target.target_type;
    if ( progress < actor_target_definite_orphan )
        return 3;
    if ( progress < actor_target_visible_enemy )
        return 4;
    if ( actor->orders.combat.shoot_at_target )
        return 6;
    if ( actor->orders.combat.throw_grenade )
        return 6;

    return 5;
}
