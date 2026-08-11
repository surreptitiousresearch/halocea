/* actor_action_try_to_dive @0x837F35D0 — attempt to make an actor dive/evade in a chosen direction. First it
 * asks actor_move_try_evasion_direction for a viable evasion (which also refines the escape direction and reports
 * whether the escape crosses a ledge); the actor must not be in a vehicle. The escape direction rotates the
 * caller's alignment vector into the actor's local frame, which is then projected onto the actor's facing to
 * produce four per-direction alignment scores. It walks global_dive_animation_table, and for every possibility
 * whose animation impulse the unit can currently play, keeps the one with the greatest (direction score + row
 * weight). If a best animation was found it re-derives the alignment vector for that animation's direction and
 * requests the impulse via actor_move_animation_impulse, firing an ai_communication_event (type 44, "diving") on
 * success.
 *
 * DEVIATION: both the DB prototype (5 args, but missing the alignment/result pointers) and the decompiler
 * (16 args, mostly spurious) get the signature wrong. Disasm (0x837F35EC-0x837F3630) settles it: r3=actor_index,
 * r4=escape_direction, f1=dive_distance, r6=alignment_vector, f2=maximum_ledge_height — escape_direction is
 * copied to a stack local whose address is passed as the in/out evade-direction reference, and the evasion
 * ledge-flag / path-collision result are stack locals. In the escape_direction > 3 branch the rotation reads the
 * still-uninitialised animation_alignment; reproduced faithfully (that direction is never produced in practice). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/real_vector2d.h"
#include "headers/path_collision_result.h"
#include "headers/dive_animation_possibility.h"
#include "headers/actor_evade.h"
#include "headers/ai_communication_type.h"
#include "headers/blam_data_globals.h"

#include "headers/ai_information_data.h"
extern dive_animation_possibility global_dive_animation_table[];

extern uint8_t actor_move_try_evasion_direction(int actor_index, real_vector2d *alignment_vector, float evade_distance, int16_t *evade_direction_reference, float maximum_ledge_height, path_collision_result *evasion_is_ledge, path_collision_result *result);
extern uint8_t unit_test_animation_impulse(int unit_index, int16_t animation_impulse);
extern uint8_t actor_move_animation_impulse(int actor_index, int16_t animation_impulse, const real_vector2d *animation_alignment);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);

uint8_t actor_action_try_to_dive(int actor_index, int16_t escape_direction, float dive_distance, real_vector2d *alignment_vector, float maximum_ledge_height)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    int16_t evade_direction = escape_direction;   /* in/out: refined by the evasion query */
    uint8_t evasion_is_ledge = 0;
    real_vector2d animation_alignment;
    path_collision_result path_result;

    if ( actor->input.vehicle_index != -1
      || !actor_move_try_evasion_direction(actor_index, alignment_vector, dive_distance, &evade_direction,
                maximum_ledge_height, (path_collision_result *)&evasion_is_ledge, &path_result) )
    {
        return 0;
    }

    /* Rotate the alignment vector into the actor's local frame per the evade direction. */
    float alignment_i;
    float alignment_j;
    switch ( (uint16_t)evade_direction )
    {
        case _actor_evade_left:
            alignment_j = alignment_vector->n[0];
            alignment_i = -alignment_vector->n[1];
            break;
        case _actor_evade_right:
            alignment_i = alignment_vector->n[1];
            alignment_j = -alignment_vector->n[0];
            break;
        case _actor_evade_forward:
        case _actor_evade_back:
            alignment_i = alignment_vector->n[0];
            alignment_j = alignment_vector->n[1];
            break;
        default:
            alignment_i = animation_alignment.n[0];   /* reads uninitialised slot; unreachable direction */
            alignment_j = animation_alignment.n[1];
            break;
    }

    /* Project the aligned vector onto the actor's facing to score each of the four dive directions. */
    float facing_i = actor->input.facing_vector.i;
    float facing_j = actor->input.facing_vector.j;
    float forward_alignment = facing_j * alignment_j + facing_i * alignment_i;
    float lateral_alignment = facing_i * alignment_j - facing_j * alignment_i;

    float direction_scores[4];
    direction_scores[0] = lateral_alignment;
    direction_scores[1] = -lateral_alignment;
    direction_scores[2] = forward_alignment;
    direction_scores[3] = -forward_alignment;

    int16_t best_animation_impulse = -1;
    int16_t best_direction = -1;
    float best_score = -0.5f;
    for ( dive_animation_possibility *possibility = global_dive_animation_table;
          possibility->animation_impulse != -1;
          ++possibility )
    {
        float score = direction_scores[possibility->animation_direction] + possibility->weight;
        if ( score > best_score
          && unit_test_animation_impulse(actor->meta.unit_index, possibility->animation_impulse) )
        {
            best_animation_impulse = possibility->animation_impulse;
            best_direction = possibility->animation_direction;
            best_score = score;
        }
    }

    if ( best_animation_impulse == -1 )
        return 0;

    /* Re-derive the alignment vector for the chosen animation's direction. */
    switch ( (uint16_t)best_direction )
    {
        case _actor_evade_left:
            animation_alignment.n[0] = alignment_j;
            animation_alignment.n[1] = -alignment_i;
            break;
        case _actor_evade_right:
            animation_alignment.n[0] = -alignment_j;
            animation_alignment.n[1] = alignment_i;
            break;
        case _actor_evade_forward:
        case _actor_evade_back:
            animation_alignment.n[0] = alignment_i;
            animation_alignment.n[1] = alignment_j;
            break;
        default:
            break;
    }

    uint8_t impulse_applied = actor_move_animation_impulse(actor_index, best_animation_impulse,
            &animation_alignment);
    if ( impulse_applied )
        ai_communication_event(_ai_communication_dive, actor->meta.unit_index, -1, -1, -1, -1, nullptr);
    return impulse_applied;
}
