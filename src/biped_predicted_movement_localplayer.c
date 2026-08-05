/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* biped_predicted_movement_localplayer @ 0x837B2508 — client-side sub-tick movement prediction for
 * the local player's biped. It copies the local player's biped object datum into a scratch buffer,
 * advances it through one *simulated* moving step (biped_update_moving_simulated), then returns the
 * position / forward / up deltas between the simulated next-tick state and the current object state,
 * scaled by the fraction of a tick that has elapsed since the last simulation tick. The renderer adds
 * these deltas to the last-tick pose so the local biped moves smoothly between 30 Hz simulation ticks.
 *
 * The scratch buffer aliases two overlapping views of a biped_datum, exactly as the original frame
 * lays them out: the copied SOURCE biped is at &scratch[32]; the simulation DESTINATION biped is at
 * &scratch[0]. Both interior fields and the object datum are reached by raw byte offset because the
 * nested object/unit/biped sub-records are not modeled. Returns 1 if a prediction was produced. */

#include <stdint.h>
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
#include "headers/unit_animation_state.h"
#include "headers/blam_data_globals.h"
extern void *memcpy(void *dst, const void *src, unsigned int size);


extern uint8_t game_time_get_paused(void);
extern float normalize3d(real_vector3d *v);
extern void biped_update_moving_simulated(unsigned int real_biped_index, biped_datum *biped_real, biped_datum *biped_faux, unit_animation_update_data *unused_animation_update);

uint8_t biped_predicted_movement_localplayer(real_point3d *position, real_vector3d *forward,
                                         real_vector3d *up, float time_since_tick_sec)
{
    unsigned char scratch[1392];          /* SOURCE biped at +32, DEST biped at +0 */
    data_iterator *iter = (data_iterator *)&scratch[16];
    player_datum *player;
    int unit_index;
    biped_datum *unit_object;
    const biped_definition *definition;
    unit_animation_update_data anim;
    double fraction;

    if ( game_time_get_paused() )
        return 0;

    data_iterator_new(iter, player_data);
    player = data_iterator_next(iter);
    if ( !player )
        return 0;

    /* find the local player (local_player_index is valid) */
    while ( (uint16_t)player->local_player_index == 0xFFFF )
    {
        player = data_iterator_next(iter);
        if ( !player )
            return 0;
    }

    unit_index = player->unit_index;
    if ( unit_index == -1 )
        return 0;

    /* object data ptr (object header datum: data ptr at int element 3*idx+2) */
    unit_object = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    definition = TAG_GET(const biped_definition, unit_object->definition_index);

    memcpy(&scratch[32], unit_object, sizeof(biped_datum));   /* SOURCE = current biped */
    scratch[0] = 0;
    scratch[1] = 0;

    /* abort if the unit is being acted upon (object +284 of source copy != -1) */
    if ( *(int *)&scratch[316] != -1 )
        return 0;

    /* if airborne / not on a movement surface, force the movement-direction vector to global forward */
    if ( (scratch[295] & 4) != 0
      || ((definition->biped.flags & (1u << _biped_flying_bit)) == 0 && (definition->biped.flags & (1u << _biped_climbs_anything_bit)) == 0) )
    {
        *(float *)&scratch[588] = 0.0f;
        if ( normalize3d((real_vector3d *)&scratch[580]) == 0.0f )
        {
            *(float *)&scratch[580] = global_forward3d->n[0];
            *(float *)&scratch[584] = global_forward3d->n[1];
            *(float *)&scratch[588] = global_forward3d->n[2];
        }
    }

    /* map the animation state byte (+707 = source biped's unit.animation.state, 0x1F4+0xA4+0x0B+32)
     * to a movement mode (+1266): idle/turning -> 0, move-front..move-right -> 1, anything else -> 2 */
    switch ( scratch[707] )
    {
        case _unit_state_idle:
        case _unit_state_turn_left:
        case _unit_state_turn_right:
            scratch[1266] = 0;
            break;
        case _unit_state_gesture:
            scratch[1266] = 2;
            break;
        default:
            if ( scratch[707] > (unsigned)_unit_state_move_right )
                scratch[1266] = 2;
            else
                scratch[1266] = 1;
            break;
    }

    /* zero a near-zero velocity vector (+664..+672) */
    if ( ((*(float *)&scratch[672] * *(float *)&scratch[672])
               + ((*(float *)&scratch[664] * *(float *)&scratch[664])
                       + (*(float *)&scratch[668] * *(float *)&scratch[668]))) < 0.010000001f )
    {
        *(float *)&scratch[664] = global_zero_vector3d->n[0];
        *(float *)&scratch[668] = global_zero_vector3d->n[1];
        *(float *)&scratch[672] = global_zero_vector3d->n[2];
    }

    /* advance the two action-hold counters (+1313/+1314) per the action bits at +1263 */
    if ( (scratch[1263] & 1) != 0 )
    {
        if ( scratch[1313] != 127 )
            ++scratch[1313];
    }
    else
    {
        scratch[1313] = 0;
    }
    if ( (scratch[1263] & 2) != 0 )
    {
        if ( scratch[1314] != 127 )
            ++scratch[1314];
    }
    else
    {
        scratch[1314] = 0;
    }

    /* destination biped header: control byte +0, crouch bit copied from source +555 */
    scratch[0] = 0;
    scratch[1] = scratch[555] & 1;

    biped_update_moving_simulated(unit_index, (biped_datum *)&scratch[32],
                                  (biped_datum *)scratch, &anim);

    /* sub-tick fraction = clamp(time_since_tick_sec * 30, 0, 1) */
    fraction = (time_since_tick_sec * 29.999998f);
    if ( fraction > 1.0 )
        fraction = 1.0;
    else if ( fraction < 0.0 )
        fraction = 0.0;

    /* delta from current object pose (position/forward/up) to the simulated next pose
     * (dest biped floats at +124..+168), scaled by the sub-tick fraction */
    position->n[0] = (*(float *)&scratch[124] - unit_object->object.position.n[0]) * (float)fraction;
    position->n[1] = (*(float *)&scratch[128] - unit_object->object.position.n[1]) * (float)fraction;
    position->n[2] = (*(float *)&scratch[132] - unit_object->object.position.n[2]) * (float)fraction;
    forward->n[0]  = (*(float *)&scratch[148] - unit_object->object.forward.n[0]) * (float)fraction;
    forward->n[1]  = (*(float *)&scratch[152] - unit_object->object.forward.n[1]) * (float)fraction;
    forward->n[2]  = (*(float *)&scratch[156] - unit_object->object.forward.n[2]) * (float)fraction;
    up->n[0]       = (*(float *)&scratch[160] - unit_object->object.up.n[0]) * (float)fraction;
    up->n[1]       = (*(float *)&scratch[164] - unit_object->object.up.n[1]) * (float)fraction;
    up->n[2]       = (*(float *)&scratch[168] - unit_object->object.up.n[2]) * (float)fraction;

    return 1;
}
