/* pre_evaluator_attack @0x837EEA50 — a firing-position pre-evaluator that biases candidate positions for the
 * attack behavior (sibling of pre_evaluator_combatmove). For each valid position, when the actor has a target it
 * adds two range terms: (1) a proximity-to-maximum-range weight that peaks (10) inside 80% of the firing
 * variant's maximum firing range and falls off as 0.8*range/distance beyond it, and (2) a combat-band weight
 * (up to 20) that rewards positions whose distance sits within the actor's preferred combat range band (berserk
 * band when the actor is berserking, clamped up by the target danger radius and the weapon's minimum target
 * range). Finally, when dangerous enemy attack vectors are present, it finds the nearest perpendicular distance
 * from the position to any type-2 (dangerous-enemy) attack line the position lies in front of, and adds a bonus
 * of 6 when clear (or no line crosses) degrading to sqrt(dist^2)*0.25 as a line gets close.
 *
 * DEVIATION: weapon_definition / actor_variant_definition are opaque forward declarations, so the range fields
 * are read at raw offsets verified from disasm: variant maximum_firing_range @0x74, combat_range_lower_bound
 * @0x9C, combat_range_upper_bound @0xA0, berserk_firing_range_lower_bound @0x168, berserk_firing_range_upper_bound
 * @0x16C; actor berserk flag byte @888. Per-step (float) casts preserved
 * for single-precision rounding. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/weapon_definition.h"
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/firing_position_attack_vector_type.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);
extern weapon_definition *actor_get_weapon_definition(int actor_index);

void pre_evaluator_attack(int actor_index, firing_position_evaluation_context *evaluation_context, int16_t firing_position_count, firing_position *firing_positions)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_variant_definition *firing_variant_definition = actor_combat_get_firing_variant_definition(actor_index);

    for ( int i = 0; i < firing_position_count; ++i )
    {
        firing_position *position = &firing_positions[i];
        if ( !position->valid )
            continue;

        if ( evaluation_context->has_target )
        {
            float distance_to_target = __fsqrts(position->linear_distance_squared_to_target);
            /* recovered: *(float *)((char *)variant + 0x74) -> ranged_combat.maximum_firing_range */
            float maximum_firing_range = firing_variant_definition->ranged_combat.maximum_firing_range;
            if ( maximum_firing_range > 0.0f )
            {
                float range_bonus;
                if ( distance_to_target >= (maximum_firing_range * 0.80000001f) )
                    range_bonus = (((maximum_firing_range * 0.80000001f) / distance_to_target)
                            * 10.0f);
                else
                    range_bonus = 10.0f;
                position->evaluation = position->evaluation + range_bonus;
            }

            /* recovered: *(float *)((char *)variant + 0xA0) -> ranged_combat.combat_range_upper_bound */
            float combat_range_upper_bound = firing_variant_definition->ranged_combat.combat_range_upper_bound;
            if ( combat_range_upper_bound > 0.0f && distance_to_target < combat_range_upper_bound )
            {
                float lower_bound;
                float upper_bound;
                if ( actor->emotions.berserk )   /* berserking */
                {
                    /* recovered: variant + 0x168/0x16C -> ranged_combat.berserk_firing_range_lower/upper_bound */
                    lower_bound = firing_variant_definition->ranged_combat.berserk_firing_range_lower_bound;
                    upper_bound = firing_variant_definition->ranged_combat.berserk_firing_range_upper_bound;
                }
                else
                {
                    /* recovered: variant + 0x9C -> ranged_combat.combat_range_lower_bound */
                    lower_bound = firing_variant_definition->ranged_combat.combat_range_lower_bound;
                    upper_bound = combat_range_upper_bound;
                }
                if ( lower_bound <= evaluation_context->target_danger_radius )
                    lower_bound = evaluation_context->target_danger_radius;

                weapon_definition *weapon = actor_get_weapon_definition(actor_index);
                if ( weapon )
                {
                    /* recovered: *(float *)((char *)weapon + 0x40C) -> weapon.ai_minimum_target_range */
                    float ai_minimum_target_range = weapon->weapon.ai_minimum_target_range;
                    if ( ai_minimum_target_range > 0.0f && lower_bound <= ai_minimum_target_range )
                        lower_bound = ai_minimum_target_range;
                }

                float slack = (upper_bound - distance_to_target);
                if ( lower_bound > 0.0f && slack > (distance_to_target - lower_bound) )
                    slack = (distance_to_target - lower_bound);

                float band_bonus = 0.0f;
                if ( slack <= 2.0f )
                {
                    if ( slack > 0.0f )
                        band_bonus = ((slack * 0.5f) * 20.0f);
                }
                else
                {
                    band_bonus = 20.0f;
                }
                position->evaluation = position->evaluation + band_bonus;
            }
        }

        if ( evaluation_context->dangerous_enemy_attack_vector_count > 0 )
        {
            float nearest_perp_dist_sq = 3.4028235e38f;   /* FLT_MAX */
            for ( int j = 0; j < evaluation_context->attack_vector_count; ++j )
            {
                firing_position_attack_vector *attack_vector = &evaluation_context->attack_vectors[j];
                if ( attack_vector->type != _firing_position_attack_vector_dangerous_enemy )
                    continue;

                firing_position_definition *definition = position->definition;
                float along = ((attack_vector->vector.n[2]
                                    * (definition->position.n[2] - attack_vector->point.n[2]))
                            + ((attack_vector->vector.n[0]
                                            * (definition->position.n[0] - attack_vector->point.n[0]))
                                    + (attack_vector->vector.n[1]
                                            * (definition->position.n[1] - attack_vector->point.n[1]))));
                if ( along > 0.0f )
                {
                    float neg_along = -along;
                    float perp_y = ((attack_vector->vector.n[1] * neg_along)
                            + (definition->position.n[1] - attack_vector->point.n[1]));
                    float perp_z = ((attack_vector->vector.n[2] * neg_along)
                            + (definition->position.n[2] - attack_vector->point.n[2]));
                    float perp_x = ((attack_vector->vector.n[0] * neg_along)
                            + (definition->position.n[0] - attack_vector->point.n[0]));
                    float perp_dist_sq = ((perp_x * perp_x)
                            + ((perp_z * perp_z) + (perp_y * perp_y)));
                    if ( perp_dist_sq < nearest_perp_dist_sq )
                        nearest_perp_dist_sq = perp_dist_sq;
                }
            }

            float attack_bonus = 6.0f;
            if ( nearest_perp_dist_sq < 12.25f )
                attack_bonus = (__fsqrts(nearest_perp_dist_sq) * 0.25f);
            position->evaluation = position->evaluation + attack_bonus;
        }
    }
}
