/* actor_move_calculate_controlled_by_aiming @0x837C7970 — pick which of four discrete facing directions an actor
 * should turn to while moving under aim control. Builds up to four unit candidate directions from the requested
 * movement vector: forward (the normalized movement, or the current facing if the movement is degenerate), its
 * reverse, and — in 2D mode only — the two perpendicular strafe directions (in 3D mode the two strafe slots are
 * left as the zero vector). Each candidate is scored by its dot product against the forced aim direction
 * (aim_dot) and against the actor's current facing (facing_dot); the candidate that best satisfies "align with
 * aim without losing too much facing" is chosen and returned as both an index (0..3) and its direction vector.
 *
 * DEVIATION: best_aim_dot/best_facing_dot are seeded in the raw decompile from an
 * uninitialized stack slot; that read is dead because best_index starts at -1 so the first iteration always
 * accepts and overwrites them. Initialized to 0.0f here. The dot-product component set (3 vs 2 terms) is gated by
 * move_in_3d per disasm (0x837C7B1C vs 0x837C7B34). */

#include <stdint.h>
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern float normalize3d(real_vector3d *v);

void actor_move_calculate_controlled_by_aiming(uint8_t move_in_3d,
        const real_vector3d *movement_vector, const real_vector3d *forced_aim_direction,
        const real_vector3d *current_facing_vector, real_vector3d *desired_facing_vector,
        int16_t *desired_facing_direction)
{
    real_vector3d candidates[4];

    if ( move_in_3d )
    {
        candidates[0] = *movement_vector;
        if ( normalize3d(&candidates[0]) == 0.0f )
            candidates[0] = *current_facing_vector;
        candidates[2] = *global_zero_vector3d;
    }
    else
    {
        candidates[0].n[0] = movement_vector->n[0];
        candidates[0].n[1] = movement_vector->n[1];
        candidates[0].n[2] = 0.0f;
        if ( normalize3d(&candidates[0]) == 0.0f )
            candidates[0] = *current_facing_vector;
        candidates[2].n[0] = -candidates[0].n[1];
        candidates[2].n[1] = candidates[0].n[0];
        candidates[2].n[2] = 0.0f;
    }

    candidates[1].n[0] = -candidates[0].n[0];
    candidates[1].n[1] = -candidates[0].n[1];
    candidates[1].n[2] = -candidates[0].n[2];
    candidates[3].n[0] = -candidates[2].n[0];
    candidates[3].n[1] = -candidates[2].n[1];
    candidates[3].n[2] = -candidates[2].n[2];

    int16_t best_index = -1;
    float best_aim_dot = 0.0f;
    float best_facing_dot = 0.0f;
    for ( int index = 0; index < 4; ++index )
    {
        const real_vector3d *candidate = &candidates[index];
        float aim_dot, facing_dot;
        if ( move_in_3d )
        {
            aim_dot = candidate->n[0] * forced_aim_direction->n[0]
                    + candidate->n[1] * forced_aim_direction->n[1]
                    + candidate->n[2] * forced_aim_direction->n[2];
            facing_dot = candidate->n[0] * current_facing_vector->n[0]
                    + candidate->n[1] * current_facing_vector->n[1]
                    + candidate->n[2] * current_facing_vector->n[2];
        }
        else
        {
            aim_dot = candidate->n[0] * forced_aim_direction->n[0]
                    + candidate->n[1] * forced_aim_direction->n[1];
            facing_dot = candidate->n[0] * current_facing_vector->n[0]
                    + candidate->n[1] * current_facing_vector->n[1];
        }

        int accept;
        if ( best_index == -1 )
            accept = 1;
        else if ( aim_dot <= best_aim_dot )
            accept = facing_dot > best_facing_dot && aim_dot > 0.5f;
        else
            accept = facing_dot > best_facing_dot || best_facing_dot < 0.5f;

        if ( accept )
        {
            best_index = (int16_t)index;
            best_aim_dot = aim_dot;
            best_facing_dot = facing_dot;
        }
    }

    *desired_facing_direction = best_index;
    *desired_facing_vector = candidates[best_index];
}
