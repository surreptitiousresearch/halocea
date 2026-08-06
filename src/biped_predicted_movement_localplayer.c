/* biped_predicted_movement_localplayer @ 0x837B2508 — client-side sub-tick movement prediction for
 * the local player's biped. It copies the local player's biped object datum into a stack-local
 * biped_datum, advances that copy through one *simulated* moving step (biped_update_moving_simulated),
 * then returns the position / forward / up deltas between the simulated next-tick state and the current
 * object state, scaled by the fraction of a tick that has elapsed since the last simulation tick. The
 * renderer adds these deltas to the last-tick pose so the local biped moves smoothly between 30 Hz
 * simulation ticks. Returns 1 if a prediction was produced.
 *
 * This is the sub-tick twin of biped_update (0x837B4028): the pre-call preparation below is the same
 * sequence biped_update runs on the live datum, applied instead to a throwaway copy.
 *
 * DEVIATION: Hex-Rays fused the three frame locals — the unit_animation_update_data at r1+0x50, the
 * data_iterator at r1+0x60 and the biped_datum copy at r1+0x70 — into one 1392-byte scratch array and
 * reached every field by raw byte offset ("local variable allocation has failed"). They are three plain
 * locals: sizeof(biped_datum) is 1360 (DB + header oracles), and 32 + 1360 == 1392 exactly, so the
 * buffer never held the "two overlapping bipeds" the old reconstruction claimed. Frame offsets read off
 * the disassembly: memcpy Dst = r1+0x70 with Size = 0x550 @0x837B25C0, and the biped_update_moving_simulated
 * call at 0x837B2788 sets only r3/r4/r5 (r5 = r1+0x50, r4 = r1+0x70) — there is no fourth argument.
 *
 * DEVIATION: the sub-tick fraction is single-precision throughout (fmuls @0x837B2798, lfs-loaded 1.0f/0.0f
 * compares @0x837B27A0/0x837B27B0); the decompiler had widened it to double and cast every use back. */

#include <stdint.h>
#include <string.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/biped_datum.h"
#include "headers/unit_animation_update_data.h"
#include "headers/global_tag_instances.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/biped_definition.h"
#include "headers/biped_definition_flags.h"
#include "headers/biped_datum_flags.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_control_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/biped_datum_state.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_time_get_paused(void);
extern float normalize3d(real_vector3d *v);
extern void biped_update_moving_simulated(unsigned int real_biped_index, biped_datum *biped,
                                          unit_animation_update_data *animation_update);

uint8_t biped_predicted_movement_localplayer(real_point3d *position, real_vector3d *forward,
                                         real_vector3d *up, float time_since_tick_sec)
{
    unit_animation_update_data animation_update;   /* frame +0x50 */
    data_iterator iter;                            /* frame +0x60 */
    biped_datum simulated;                         /* frame +0x70 */
    player_datum *player;
    int unit_index;
    biped_datum *unit_object;
    const biped_definition *definition;
    uint8_t animation_state;
    float fraction;

    if ( game_time_get_paused() )
        return 0;

    data_iterator_new(&iter, player_data);
    player = data_iterator_next(&iter);
    if ( !player )
        return 0;

    /* find the local player (local_player_index is valid) */
    while ( (uint16_t)player->local_player_index == 0xFFFF )
    {
        player = data_iterator_next(&iter);
        if ( !player )
            return 0;
    }

    unit_index = player->unit_index;
    if ( unit_index == -1 )
        return 0;

    /* object data ptr (object header datum: data ptr at int element 3*idx+2) */
    unit_object = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    definition = TAG_GET(const biped_definition, unit_object->definition_index);

    memcpy(&simulated, unit_object, sizeof(biped_datum));
    animation_update.state_desired = 0;
    animation_update.crouching = 0;

    /* a seated / carried biped is driven by its parent, so there is nothing to predict */
    if ( simulated.object.parent_object_index != -1 )
        return 0;

    /* dead, or a ground-bound biped: flatten the desired facing vector into the XY plane and fall back
     * to global forward if it degenerates (same block as biped_update @0x837B41EC) */
    if ( (simulated.object.damage_flags & (1u << _object_dead_bit)) != 0
      || ((definition->biped.flags & (1u << _biped_flying_bit)) == 0 && (definition->biped.flags & (1u << _biped_climbs_anything_bit)) == 0) )
    {
        simulated.unit.desired_facing_vector.n[2] = 0.0f;
        if ( normalize3d(&simulated.unit.desired_facing_vector) == 0.0f )
        {
            simulated.unit.desired_facing_vector.n[0] = global_forward3d->n[0];
            simulated.unit.desired_facing_vector.n[1] = global_forward3d->n[1];
            simulated.unit.desired_facing_vector.n[2] = global_forward3d->n[2];
        }
    }

    /* classify unit.animation.state into biped.state, the same mapping biped_update @0x837B4220
     * applies to the live datum */
    animation_state = (uint8_t)simulated.unit.animation.state;
    switch ( animation_state )
    {
        case _unit_state_idle:
        case _unit_state_turn_left:
        case _unit_state_turn_right:
            simulated.biped.state = biped_state_idle;
            break;
        case _unit_state_gesture:
            simulated.biped.state = biped_state_unknown;
            break;
        default:
            if ( animation_state > (uint8_t)_unit_state_move_right )
                simulated.biped.state = biped_state_unknown;
            else
                simulated.biped.state = biped_state_moving;
            break;
    }

    /* squelch sub-threshold throttle input */
    if ( ((simulated.unit.throttle.n[2] * simulated.unit.throttle.n[2])
               + ((simulated.unit.throttle.n[0] * simulated.unit.throttle.n[0])
                       + (simulated.unit.throttle.n[1] * simulated.unit.throttle.n[1]))) < 0.010000001f )
    {
        simulated.unit.throttle.n[0] = global_zero_vector3d->n[0];
        simulated.unit.throttle.n[1] = global_zero_vector3d->n[1];
        simulated.unit.throttle.n[2] = global_zero_vector3d->n[2];
    }

    /* age the airborne / slipping tick counters from the biped physics flags */
    if ( (simulated.biped.flags & (1u << _biped_airborne_bit)) != 0 )
    {
        if ( simulated.biped.airborne_ticks != 127 )
            ++simulated.biped.airborne_ticks;
    }
    else
    {
        simulated.biped.airborne_ticks = 0;
    }
    if ( (simulated.biped.flags & (1u << _biped_slipping_bit)) != 0 )
    {
        if ( simulated.biped.slipping_ticks != 127 )
            ++simulated.biped.slipping_ticks;
    }
    else
    {
        simulated.biped.slipping_ticks = 0;
    }

    animation_update.state_desired = 0;
    animation_update.crouching = simulated.unit.control_flags & (1u << _unit_control_crouch_modifier_bit);

    biped_update_moving_simulated(unit_index, &simulated, &animation_update);

    /* sub-tick fraction = clamp(time_since_tick_sec / SECONDS_PER_TICK, 0, 1). The literal is the
     * reciprocal 1.0f/SECONDS_PER_TICK the compiler folded in: 0x41EFFFFF, which is NOT 30.0f
     * (0x41F00000), so TICKS_PER_SECOND cannot be substituted here. */
    fraction = (time_since_tick_sec * 29.999998f);
    if ( fraction > 1.0f )
        fraction = 1.0f;
    else if ( fraction < 0.0f )
        fraction = 0.0f;

    /* delta from the current object pose to the simulated next-tick pose, scaled by the sub-tick fraction */
    position->n[0] = (simulated.object.position.n[0] - unit_object->object.position.n[0]) * fraction;
    position->n[1] = (simulated.object.position.n[1] - unit_object->object.position.n[1]) * fraction;
    position->n[2] = (simulated.object.position.n[2] - unit_object->object.position.n[2]) * fraction;
    forward->n[0]  = (simulated.object.forward.n[0] - unit_object->object.forward.n[0]) * fraction;
    forward->n[1]  = (simulated.object.forward.n[1] - unit_object->object.forward.n[1]) * fraction;
    forward->n[2]  = (simulated.object.forward.n[2] - unit_object->object.forward.n[2]) * fraction;
    up->n[0]       = (simulated.object.up.n[0] - unit_object->object.up.n[0]) * fraction;
    up->n[1]       = (simulated.object.up.n[1] - unit_object->object.up.n[1]) * fraction;
    up->n[2]       = (simulated.object.up.n[2] - unit_object->object.up.n[2]) * fraction;

    return 1;
}
