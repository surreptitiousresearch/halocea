/* pre_evaluator_pursuit @0x837EE650 — pre-evaluation pass for "pursuit" firing-position scoring: for
 * every valid candidate, scores it higher the further it is beyond half the maximum allowable range (up
 * to a flat +5 bonus once beyond the full range), with an extra bonus for being close to the target
 * (within 20 units) and, when direction-from-target scoring is enabled and the target has a hint vector,
 * an additional bonus scaled by how well the position's path-direction-from-target aligns with that hint
 * vector (a alignment bonus, zeroed out for opposing alignments).
 *
 * DEVIATION: the decompiler renders the target-hint-vector dot product through several raw offsets
 * (path_distance_from_actor+{4,6,7,8,12,40} as bytes/floats past a rebased pointer); all resolved against
 * firing_position.h's DB layout — they are path_distance_to_target, path_direction_from_target.n[0..2],
 * evaluation, and valid respectively. The `fsel f13,f0,f0,f10` inline asm is the standard "clamp to >= 0"
 * idiom (f10 assumed preloaded with 0.0, the only value that makes the surrounding math sensible),
 * simplified to a plain `>= 0.0f ? x : 0.0f`. */

#include <stdint.h>
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"

extern double __fabs(double x);

void pre_evaluator_pursuit(int actor_index, firing_position_evaluation_context *evaluation_context, int16_t firing_position_count, firing_position *firing_positions)
{
    for ( int16_t i = 0; i < firing_position_count; ++i )
    {
        firing_position *entry = &firing_positions[i];
        if ( !entry->valid )
            continue;

        float range_bonus;
        float half_range = evaluation_context->maximum_allowable_range * 0.5f;

        if ( entry->path_distance_from_actor >= half_range )
        {
            range_bonus = (entry->path_distance_from_actor < evaluation_context->maximum_allowable_range)
                ? (1.0f / half_range) * (evaluation_context->maximum_allowable_range - entry->path_distance_from_actor) * 5.0f
                : 5.0f;
        }
        else
        {
            range_bonus = 5.0f;
        }

        entry->evaluation += range_bonus;

        if ( !evaluation_context->has_target )
            continue;

        if ( entry->path_distance_to_target < 20.0f )
            entry->evaluation = (20.0f - entry->path_distance_to_target) * 0.5f + (entry->evaluation);

        if ( evaluation_context->find_path_direction_from_target && evaluation_context->target_has_hint_vector )
        {
            float alignment = entry->path_direction_from_target.n[0] * evaluation_context->target_hint_vector.n[0]
                             + entry->path_direction_from_target.n[1] * evaluation_context->target_hint_vector.n[1]
                             + entry->path_direction_from_target.n[2] * evaluation_context->target_hint_vector.n[2];

            float alignment_bonus;
            if ( alignment <= 0.70710677f )
            {
                float scaled = alignment * 1.4142135f;
                float clamped = (scaled >= 0.0f) ? scaled : 0.0f;
                alignment_bonus = clamped * 10.0f;
            }
            else
            {
                alignment_bonus = 10.0f;
            }

            entry->evaluation += alignment_bonus;
        }
    }
}
