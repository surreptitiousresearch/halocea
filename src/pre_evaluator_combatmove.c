/* pre_evaluator_combatmove @0x837EED08 — a firing-position pre-evaluator that biases candidate positions for
 * combat movement. First, when the actor has a target within its firing variant's maximum firing range, it adds
 * a range-proximity weight (up to 8, scaled by how far inside the range the target is) to each reachable
 * position whose path distance is within the allowable range. Second, when friendly attack vectors are present,
 * it tests each valid position against the query's attack vectors and adds a bonus based on the worst blockage
 * result across type-1 (friendly) and type-0 vectors: fully clear positions score highest (10), degrading
 * through 8.5/6/1.5 down to 0 as blockage worsens.
 *
 * DEVIATION: the `fsel f9, f10, f10, f31` with f31=0 (disasm 0x837EED48/0x837EEDB0) is max(0, 1 - path_dist/
 * max_range). The firing variant's maximum_firing_range is at offset 0x74 (ranged_combat @ 0x64 + 0x10, disasm
 * 0x837EED54); actor_ranged_combat_properties is not modeled, so it is read as a raw float. */

#include <stdint.h>
#include "headers/actor_variant_definition.h"
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/firing_position_attack_vector_type.h"

#include "headers/real_vector3d.h"
extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);
extern int16_t actor_perception_aiming_vector_test_blockage(const real_point3d *source_position, const real_vector3d *source_vector, const real_point3d *friend_position, real_vector3d *friend_direction_to_aiming_vector);

void pre_evaluator_combatmove(int actor_index, firing_position_evaluation_context *evaluation_context, int16_t firing_position_count, firing_position *firing_positions)
{
    actor_variant_definition *firing_variant_definition = actor_combat_get_firing_variant_definition(actor_index);
    /* recovered: *(float *)((char *)variant + 0x74) -> ranged_combat.maximum_firing_range */
    float maximum_firing_range = firing_variant_definition->ranged_combat.maximum_firing_range;

    float range_weight = 8.0f;
    if ( !evaluation_context->has_target
      || (evaluation_context->target_current_distance <= maximum_firing_range
          && (range_weight = (1.0f - evaluation_context->target_current_distance / maximum_firing_range) * 8.0f) > 0.0f) )
    {
        for ( int i = 0; i < firing_position_count; ++i )
        {
            firing_position *position = &firing_positions[i];
            if ( position->valid
              && position->path_distance_from_actor < evaluation_context->maximum_allowable_range )
            {
                float proximity = 1.0f - position->path_distance_from_actor / evaluation_context->maximum_allowable_range;
                if ( proximity < 0.0f )
                    proximity = 0.0f;   /* fsel(proximity, proximity, 0) == max(0, proximity) */
                position->evaluation = proximity * range_weight + position->evaluation;
            }
        }
    }

    if ( evaluation_context->friend_attack_vector_count > 0 )
    {
        for ( int i = 0; i < firing_position_count; ++i )
        {
            firing_position *position = &firing_positions[i];
            if ( !position->valid )
                continue;

            __int16 worst_friendly_blockage = 0;
            __int16 worst_enemy_blockage = 0;
            for ( int j = 0; j < evaluation_context->attack_vector_count; ++j )
            {
                firing_position_attack_vector *attack_vector = &evaluation_context->attack_vectors[j];
                if ( attack_vector->type != _firing_position_attack_vector_friend
                  && attack_vector->type != _firing_position_attack_vector_friend_player )
                    continue;

                __int16 blockage = actor_perception_aiming_vector_test_blockage(&attack_vector->point,
                        &attack_vector->vector, (const real_point3d *)position->definition, nullptr);
                if ( attack_vector->type )
                {
                    if ( worst_friendly_blockage < blockage )
                        worst_friendly_blockage = blockage;
                }
                else if ( worst_enemy_blockage < blockage )
                {
                    worst_enemy_blockage = blockage;
                }
            }

            float bonus;
            if ( worst_friendly_blockage >= 2 )
                bonus = 0.0f;
            else if ( worst_friendly_blockage >= 1 )
                bonus = 1.5f;
            else if ( worst_enemy_blockage >= 2 )
                bonus = 6.0f;
            else if ( worst_enemy_blockage >= 1 )
                bonus = 8.5f;
            else
                bonus = 10.0f;
            position->evaluation = position->evaluation + bonus;
        }
    }
}
