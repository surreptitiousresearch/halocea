/* action_charge_find_target_range @0x837F9080 — pick the acceptable engagement range for a charge target,
 * depending on the charge goal. For melee goals (melee / melee_leaping): the range is the base melee
 * range (actor_definition.berserk.melee_attack_range, extended by state_data->melee_danger_range unless
 * melee_suicide is set), and for melee_leaping additionally at least the leap upper bound
 * (berserk.melee_leap_range_upper_bound) when non-negative. For ranged goals (vehicle_strafing / 0): if
 * the actor has a ranged weapon and an acknowledged-or-better target, uses its preferred engagement
 * range (control.weapon_maximum_range) when non-negative; otherwise 0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/charge_state_data.h"
#include "headers/actor_target_type.h"
#include "headers/charge_goal.h"
#include "headers/blam_data_globals.h"

extern uint8_t actor_has_ranged_weapon(int actor_index);

float action_charge_find_target_range(int actor_index, int16_t goal, charge_state_data *state_data)
{
    float range = 0.0f;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( goal == charge_goal_melee || goal == charge_goal_melee_leaping )
    {
        actor_definition *definition =
            TAG_GET(actor_definition, actor->meta.definition_index);

        if ( goal == charge_goal_melee_leaping && definition->berserk.melee_leap_range_upper_bound >= 0.0f )
            range = definition->berserk.melee_leap_range_upper_bound;

        float melee_range = definition->berserk.melee_attack_range;
        if ( !state_data->melee_suicide )
            melee_range = definition->berserk.melee_attack_range + state_data->melee_danger_range;

        if ( range <= melee_range )
            range = melee_range;
    }
    else if ( goal == charge_goal_vehicle_strafing || !goal )
    {
        if ( actor_has_ranged_weapon(actor_index) && actor->target.target_type >= actor_target_acknowledged_enemy )
        {
            float preferred_range = actor->control.weapon_maximum_range;
            if ( preferred_range >= 0.0f )
                range = preferred_range;
        }
    }

    return range;
}
