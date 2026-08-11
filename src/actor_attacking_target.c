/* actor_attacking_target @0x8371DC48 — if an actor is actively attacking (engagement level past the
 * threshold and its current action class is "active"), output the direction of its attack and return 1.
 * When the actor has a discrete shot-target vector that is used; otherwise its aiming vector is used,
 * provided it has a valid target. Returns 0 when the actor is not attacking. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/real_vector3d.h"
#include "headers/actor_target_type.h"
#include "headers/actor_fire_target_type.h"
#include "headers/action_class.h"
#include "headers/blam_data_globals.h"

extern int16_t actor_action_class(int actor_index);

int actor_attacking_target(int actor_index, real_vector3d *attack_vector)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if (actor->target.target_type <= actor_target_clear_line_of_sight_enemy || actor_action_class(actor_index) != _action_class_active)
        return 0;

    if (actor->control.grenade_trying_to_throw) /* has a discrete shot-target vector */
    {
        attack_vector->n[0] = actor->input.aiming_vector.n[0];
        attack_vector->n[1] = actor->input.aiming_vector.n[1];
        attack_vector->n[2] = actor->input.aiming_vector.n[2];
        return 1;
    }

    if (actor->control.current_fire_target_type <= actor_fire_target_none) /* no valid target */
        return 0;

    attack_vector->n[0] = actor->control.current_fire_target_aim_vector.n[0];
    attack_vector->n[1] = actor->control.current_fire_target_aim_vector.n[1];
    attack_vector->n[2] = actor->control.current_fire_target_aim_vector.n[2];
    return 1;
}
