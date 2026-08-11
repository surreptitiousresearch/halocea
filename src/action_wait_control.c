/* action_wait_control @0x83822AA8 — enter the "wait" action for an actor: picks a wait-state variant based on
 * whether the actor is scripted to wait forever (control.moving), has no destination
 * (external_orders.stand_down / no path / action_data+12 time left), or has a live path target index
 * (external_orders.pursuit_group_prop_index) to resume waiting at. Always resets the sub-action progress
 * fields. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/idle_look_type.h"
#include "headers/blam_data_globals.h"


void action_wait_control(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->control.moving )
    {
        actor->orders.look.primary_priority = _primary_priority_facing;
        actor->orders.look.primary_direction.type = _direction_specification_movement;
    }
    else
    {
        int saved_path_target = actor->external_orders.pursuit_group_prop_index;
        if ( actor->external_orders.stand_down || saved_path_target == -1 || actor->state.action_data.___u0.wait.look_timer <= 0 )
        {
            actor->orders.look.primary_priority = _primary_priority_face_360;
        }
        else
        {
            actor->orders.look.primary_direction.___u1.prop_index = saved_path_target;
            actor->orders.look.primary_priority = _primary_priority_aiming;
            actor->orders.look.primary_direction.type = _direction_specification_prop;
        }
    }

    actor->orders.look.idle_look_type = _idle_look_searching;
    actor->orders.combat.shoot_at_target = 0;
    actor->orders.move.stationary_crouch = 0;
    actor->orders.move.moving_crouch = 0;
    actor->orders.move.panicked = 0;
    actor->orders.move.dive_into_cover = 0;
    actor->orders.move.emerge_from_cover = 0;
}
