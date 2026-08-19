/* pre_evaluator_panic @0x837EE7A8 — the "panic" firing-position pre-evaluator: a fast pre-scoring pass over a
 * candidate firing-position list that biases toward positions at a comfortable range and, when a target is
 * known, toward positions with a reasonable line to it. Positions closer than 4 world units are rejected;
 * between 4 and 8 they gain a linear bonus, past 8 a bonus that fades toward the maximum allowable range. With a
 * target, positions inside 4 units (dist² < 16) are rejected, farther ones gain a distance bonus (capped at
 * range 7), and a final proximity term rewards being nearer the target than the actor currently is — but a
 * position much closer than the actor (proximity > 0.5) is rejected. Rejected positions are dropped entirely
 * unless the context allows rejected positions to remain (flagged). Scores accumulate into firing_position.evaluation.
 *
 * DEVIATION: register-pun doubles are plain float math; the single advancing pointer is expressed as an indexed
 * firing_position walk. The rejected-flag writes are compiler side effects of the shipped short-circuit
 * conditions and are reproduced at the point they occur. */

#include <stdint.h>
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"

extern float __fsqrts(float);

void pre_evaluator_panic(int actor_index, firing_position_evaluation_context *evaluation_context, int16_t firing_position_count, firing_position *firing_positions)
{
    for ( int i = 0; i < firing_position_count; i++ )
    {
        firing_position *fp = &firing_positions[i];
        if ( !fp->valid )
            continue;

        float distance = fp->path_distance_from_actor;
        if ( distance < 4.0f )
        {
            fp->rejected = 1;
            if ( !evaluation_context->allow_rejected_positions )
            {
                fp->valid = 0;
                continue;
            }
        }
        else
        {
            float range_bonus = 0.0f;
            if ( distance >= 8.0f )
            {
                if ( distance < evaluation_context->maximum_allowable_range )
                    range_bonus = (evaluation_context->maximum_allowable_range - fp->path_distance_from_actor) * 8.0f
                                / (evaluation_context->maximum_allowable_range - 8.0f);
            }
            else
            {
                range_bonus = (fp->path_distance_from_actor - 4.0f) * 2.0f;
            }
            fp->evaluation = fp->evaluation + range_bonus;
        }

        if ( !evaluation_context->has_target )
            continue;

        float distance_squared_to_target = fp->linear_distance_squared_to_target;
        if ( distance_squared_to_target < 16.0f )
        {
            fp->rejected = 1;
            if ( !evaluation_context->allow_rejected_positions )
            {
                fp->valid = 0;
                continue;
            }
        }

        float target_bonus;
        if ( distance_squared_to_target >= 16.0f )
        {
            if ( distance_squared_to_target >= 49.0f )
                target_bonus = 10.0f;
            else
                target_bonus = (__fsqrts(distance_squared_to_target) - 4.0f) * 3.3333333f;
        }
        else
        {
            target_bonus = 0.0f;
        }

        float closest_approach = fp->path_closest_approach_to_target;
        float evaluation_with_target = fp->evaluation + target_bonus;
        fp->evaluation = fp->evaluation + target_bonus;

        if ( closest_approach < 3.4028235e38f )
        {
            float target_current_distance = evaluation_context->target_current_distance;
            if ( target_current_distance > 0.0f && target_current_distance < 3.4028235e38f )
            {
                float proximity = 1.0f
                        - closest_approach / (evaluation_context->target_current_distance * 0.80000001f);
                if ( proximity > 0.5f )
                {
                    fp->rejected = 1;
                    if ( !evaluation_context->allow_rejected_positions )
                    {
                        fp->valid = 0;
                        continue;
                    }
                }
                if ( proximity < 0.0f )
                    proximity = 0.0f;
                else if ( proximity > 1.0f )
                    proximity = 1.0f;
                fp->evaluation = (1.0f - proximity) * 8.0f + evaluation_with_target;
            }
        }
    }
}
