/* action_uncover_control @0x83824EB0 — enter/refresh the "uncover" action for an actor (sibling of
 * action_wait_control.c). When the actor has a target prop (+624 != -1), it decides whether the actor has
 * "reached" a position from which to uncover the target: in uncover-mode 0 this is gated on either a scripted
 * flag (+354) or a per-difficulty timer threshold (+616 vs 5, or 6 when the actor's tag lacks flag 0x10);
 * combined with the prop's line-of-sight state (a hidden/barely-seen target counts as reached). It then picks
 * the sub-action state word (+1000): 7 if reached, else 2/5 depending on the target's line-of-sight when the
 * timer is up, else 3. Uncover-mode selects the approach variant (+1004) and, for mode 1, snapshots the
 * stored target point (+176..+184 -> +1008..+1016). Finally the common sub-action progress scratch fields are
 * reset.
 *
 * Fields resolved to named actor_datum members and the DB-named uncover_state_data arm of action_data
 * (target prop index +624, uncover mode = uncover arm +8, sub-action state block +1000.., etc.). The prop's line-of-sight uses prop_datum.h. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_target_type.h"
#include "headers/uncover_state_data.h"
#include "headers/actor_definition_flags.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/idle_look_type.h"
#include "headers/ai_line_of_sight.h"
#include "headers/blam_data_globals.h"


void action_uncover_control(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    uncover_state_data *action_data = &actor->state.action_data.___u0.uncover;

    int target_prop_index = actor->target.target_prop_index;
    if ( target_prop_index != -1 )
    {
        unsigned __int8 reached = 0;
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, target_prop_index);

        __int16 uncover_mode = actor->state.action_data.___u0.uncover.pursuit_location.type;
        if ( !uncover_mode )
        {
            if ( actor->input.vehicle_gunner_bombardment )
            {
                actor->orders.combat.shoot_at_target = 1;
                reached = 1;
                actor->orders.combat.bombard_target = 1;
            }
            else
            {
                __int16 threshold = actor_target_uninspected_orphan;
                /* actor ('actr') definition flags@0x00; bit 4 = shoot at target's last location */
                if ( (*TAG_GET(int, actor->meta.definition_index) & (1u << _actor_definition_shoot_at_targets_last_location_bit)) == 0 )
                    threshold = actor_target_definite_orphan;
                actor->orders.combat.shoot_at_target = actor->target.target_type >= threshold;
            }
        }

        if ( actor->orders.combat.shoot_at_target && (prop->line_of_sight == _ai_line_of_sight_clear || prop->line_of_sight == _ai_line_of_sight_occluded) )
            reached = 1;

        if ( reached )
        {
            actor->orders.look.primary_priority = _primary_priority_locked_aiming;
        }
        else if ( actor->target.target_type >= actor_target_uninspected_orphan )
        {
            __int16 line_of_sight = prop->line_of_sight;
            if ( line_of_sight == _ai_line_of_sight_from_cover || line_of_sight == _ai_line_of_sight_obstructed )
                actor->orders.look.primary_priority = _primary_priority_opportunity_aiming;
            else
                actor->orders.look.primary_priority = _primary_priority_aiming;
        }
        else
        {
            actor->orders.look.primary_priority = _primary_priority_facing;
        }

        if ( uncover_mode )
        {
            if ( uncover_mode == 1 )
            {
                actor->orders.look.primary_direction.type = _direction_specification_point;
                actor->orders.look.primary_direction.___u1.prop_index = *(int *)&actor->state.action_data.___u0.uncover.pursuit_location.position.x;
                *(int *)&actor->orders.look.primary_direction.___u1.point.y = *(int *)&actor->state.action_data.___u0.uncover.pursuit_location.position.y;
                *(int *)&actor->orders.look.primary_direction.___u1.point.z = *(int *)&actor->state.action_data.___u0.uncover.pursuit_location.position.z;
            }
        }
        else
        {
            actor->orders.look.primary_direction.type = _direction_specification_target;
        }
    }

    actor->orders.look.idle_look_type = _idle_look_searching;
    actor->orders.move.stationary_crouch = action_data->sneaking;
    char sub_action_progress = action_data->sneaking;
    actor->orders.move.panicked = 0;
    actor->orders.move.dive_into_cover = 1;
    actor->orders.move.emerge_from_cover = 0;
    actor->orders.move.moving_crouch = sub_action_progress;
}
