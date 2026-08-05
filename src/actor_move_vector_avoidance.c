/*
 * actor_move_vector_avoidance @ 0x837C93B8  (size 3312)
 *
 * CAVEAT (large frame / stack-slot reuse):
 *   The decompiler flagged "positive sp value has been detected" (this frame is
 *   ~0x6230 bytes) and fused several independent stack slots.  Untangled here:
 *     - The 2-halfword avoidance-ray hit buffer the decompiler overlaid on the
 *       out_fraction float slot is recovered as the separate avoid_hit[2] local
 *       (disasm: sthx of the actor_move_test_avoidance_vector result at
 *       arg_54 + 2*side, later lhzx compares).
 *     - The 64-bit "clear_time_packed" slot (high dword = byte offset into the
 *       hit buffer, low dword = the clear-time byte) is recovered as plain
 *       indexing plus the clear_time local; the decompiler's 64-bit store into
 *       the sense_point slot was the std/lfd/fcfid integer->float conversion
 *       idiom (0x837C97C0), not a data store, and is dropped.
 *   RtlCheckStack12() is the compiler stack-probe for the large frame and is
 *   dropped; the datum index comes directly from actor_index.
 */

#include <stdint.h>
#include "headers/ppc_intrinsics.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/object_datum.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/vector_avoidance_data.h"
#include "headers/vector_avoidance_ray.h"
#include "headers/structure_bsp.h"
#include "headers/collision_bsp.h"
#include "headers/blam_data_globals.h"

/* --- extern boundaries (siblings / globals) --------------------------------- */

extern float sense_ray_avoidance_weights[9][8];
extern float avoid_ray_avoidance_weights[2];
extern float avoid_ray_adjacent_fractions[2];   /* @0x82126790: spread onto +-1 / +-2 neighbours */

extern void actor_move_avoidance_setup(vector_avoidance_data *avoidance_data);
extern int16_t actor_move_test_avoidance_vector(vector_avoidance_data *avoidance_data, const vector_avoidance_ray *avoidance_ray, real_point3d *ray_origin, real_vector3d *ray_direction, float *collision_t, uint8_t *collision_timer);
extern uint8_t actor_move_vector_avoidance_find_direction(int16_t direction_count, const real_vector3d *avoidance_directions, const float *avoidance_weights, const real_vector3d *direction_vector, float *approximate_direction_reference, float *approximate_weight_reference);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern float angle_between_vectors3d(const real_vector3d *a, const real_vector3d *b);

void actor_move_vector_avoidance(
        int actor_index,
        const real_vector3d *movement_direction,
        real_vector3d *avoidance_rotation,
        float *emergency_amount)
{
    actor_datum *actor;
    object_datum *unit_object;      /* object_header .data pointer for the actor's unit */
    int object_index;
    uint8_t result_written;

    float sense_emergency;
    float sense_clear_fraction, sense_weight_scale, sense_weight_clamped;
    int weight_index_next, weight_index_back, weight_index;
    int i, j;
    int16_t last_direction;
    float half_seed_weight;

    /* left = forward x up cross product, seeded into avoidance_data.left */
    float left_cross_x, left_cross_y, left_cross_z;

    int avoid_dir_index, side_index, next_side_index, weight_side_index;
    float avoid_weight, clear_time_fraction, blocked_weight_scale;
    uint8_t blocked_flag;
    uint32_t clear_time;                 /* per-side clear-time byte, zero-extended */
    int16_t avoid_hit[2];                /* per-side hit results of the ray pair */
    int weight_index_fwd1, weight_index_fwd2, next_avoid_dir, weight_index_back1, weight_index_back2;

    /* object up/left basis reloaded for the velocity-bias + move transform */
    float up_x, up_y, up_z, left_x, left_y, left_z;
    float velocity_speed;
    uint8_t found_direction;
    float velocity_bias_fraction, velocity_bias_weight;
    float vel_plane_x, vel_dot_up, vel_dot_left_neg, vel_plane_mag;
    real_vector3d *direction;

    int16_t best_index;
    int16_t dir_scan_index, current_dir_index;
    float best_weight;
    float move_z, move_x, zero_y, zero_z;
    float move_fit_fraction, move_dot_forward, move_y, move_mag_sq, move_mag;
    float move_norm_x, move_norm_y, move_norm_z;
    bool move_mag_nonpositive;
    float move_plane_x, move_plane_y, move_plane_z, move_plane_mag, weight_advantage;
    uint8_t is_panicking;
    float emergency_scaled;
    int panic_timer;
    bool should_panic;
    int panic_active, panic_timer_current;
    float panic_zero_y, panic_zero_z;
    real_vector3d *best_direction;
    float best_dir_x, world_z_acc, best_dir_y, world_x_acc, world_z_acc2, best_dir_z;
    float world_y_acc, move_dir_x, move_dir_y, move_dir_z;
    float cross_y, cross_z, cross_x, cross_mag;
    float axis_x, axis_y, axis_z, rotation_angle, panic_emergency;
    real_vector3d *slide_direction;
    float slide_emergency, slide_angle, rotation_y_comp, rotation_z_comp;
    float zero_x, nudge_zero_y;
    float best_dir_z2, best_dir_y2, rot_z_acc, rot_y_acc, rot_mag, rot_y_norm, rot_z_norm;
    float final_rotation_y, final_rotation_z;

    float out_emergency;                 /* -> *emergency_amount */
    float out_fraction;                  /* collision_t / approximate-weight scratch */
    float rotation_x;
    float rotation_y;
    float rotation_z;
    real_vector3d scratch_vector;
    real_vector3d world_direction;
    float direction_weights[8];          /* zero-initialised */
    real_point3d sense_point;
    real_vector3d avoid_result_vector;
    real_point3d avoid_point;
    vector_avoidance_data avoidance_data;

    uint16_t actor_datum_index = (uint16_t)actor_index;

    result_written = 0;
    actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_datum_index);
    out_emergency = 0.0f;
    rotation_x = global_zero_vector3d->n[0];
    rotation_y = global_zero_vector3d->n[1];
    rotation_z = global_zero_vector3d->n[2];

    object_index = actor->input.vehicle_index;
    if ( object_index == -1 )
    {
        object_index = actor->meta.unit_index;
        if ( object_index == -1 )
            goto LABEL_109;
    }

    unit_object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
    avoidance_data.structure = global_structure_bsp;
    avoidance_data.bsp = global_collision_bsp;
    avoidance_data.object_index = object_index;
    object_get_origin(object_index, &avoidance_data.origin);

    avoidance_data.forward.n[0] = unit_object->object.forward.n[0];
    avoidance_data.forward.n[1] = unit_object->object.forward.n[1];
    avoidance_data.forward.n[2] = unit_object->object.forward.n[2];
    avoidance_data.up.n[0] = unit_object->object.up.n[0];
    avoidance_data.up.n[1] = unit_object->object.up.n[1];
    avoidance_data.up.n[2] = unit_object->object.up.n[2];
    left_cross_y = ((unit_object->object.up.n[2] * unit_object->object.forward.n[0]) - (unit_object->object.forward.n[2] * unit_object->object.up.n[0]));
    left_cross_z = ((unit_object->object.up.n[0] * unit_object->object.forward.n[1]) - (unit_object->object.forward.n[0] * unit_object->object.up.n[1]));
    left_cross_x = ((unit_object->object.forward.n[2] * unit_object->object.up.n[1]) - (unit_object->object.up.n[2] * unit_object->object.forward.n[1]));
    avoidance_data.avoid_distance = 12.0f;
    avoidance_data.avoid_width = 1.0f;
    avoidance_data.left.n[2] = left_cross_z;
    avoidance_data.left.n[1] = left_cross_y;
    avoidance_data.left.n[0] = left_cross_x;
    actor_move_avoidance_setup(&avoidance_data);

    sense_emergency = 0.0f;
    /* zero-initialise the 8 direction weights */
    direction_weights[0] = 0.0f; direction_weights[1] = 0.0f;
    direction_weights[2] = 0.0f; direction_weights[3] = 0.0f;
    direction_weights[4] = 0.0f; direction_weights[5] = 0.0f;
    direction_weights[6] = 0.0f; direction_weights[7] = 0.0f;

    /* seed weights biased around the last chosen direction */
    last_direction = actor->control.vector_avoidance_current_direction;
    if ( (unsigned int)last_direction < 8 )
    {
        half_seed_weight = avoid_ray_adjacent_fractions[1] * 0.4f;
        weight_index_next = (last_direction + 1) % 8;
        direction_weights[last_direction] = direction_weights[last_direction] + 0.4f;
        direction_weights[weight_index_next] = direction_weights[weight_index_next] + 0.32f;
        direction_weights[(last_direction + 2) % 8] =
            half_seed_weight + direction_weights[(last_direction + 2) % 8];
        direction_weights[(last_direction + 7) % 8] =
            direction_weights[(last_direction + 7) % 8] + 0.32f;
        weight_index_back = (last_direction + 6) % 8;
        direction_weights[weight_index_back] =
            half_seed_weight + direction_weights[weight_index_back];
    }

    /* sense rays: accumulate emergency + directional weights */
    for ( i = 0; i < 9; i = (int16_t)(i + 1) )
    {
        if ( actor_move_test_avoidance_vector(&avoidance_data, &sense_rays[i],
                                              &sense_point, &scratch_vector, &out_fraction,
                                              0) > 0 )
        {
            sense_clear_fraction = (1.0f - out_fraction);
            weight_index = 0;
            sense_weight_scale = ((1.0f - out_fraction) * 2.0f);
            do
            {
                sense_weight_clamped = sense_weight_scale;
                if ( sense_weight_scale > 1.0f )
                    sense_weight_clamped = 1.0f;
                direction_weights[weight_index] =
                    (sense_ray_avoidance_weights[i][weight_index] * sense_weight_clamped)
                    + direction_weights[weight_index];
                weight_index = (int16_t)(weight_index + 1);
            }
            while ( weight_index < 8 );
            if ( sense_emergency <= sense_clear_fraction )
                sense_emergency = sense_clear_fraction;
        }
    }

    /* avoidance rays (8 directions x 2 sides) */
    avoid_dir_index = 0;
    do
    {
        side_index = 0;
        do
        {
            avoid_hit[side_index] = actor_move_test_avoidance_vector(
                    &avoidance_data,
                    &avoidance_rays[avoid_dir_index][side_index],
                    &avoid_point,
                    &avoid_result_vector,
                    &scratch_vector.n[side_index],
                    &actor->control.vector_avoidance_clear_times[avoid_dir_index][side_index]);
            next_side_index = (int16_t)(side_index + 1);
            side_index = next_side_index;
        }
        while ( next_side_index < 2 );

        weight_side_index = 1;
        avoid_weight = 0.0f;
        blocked_flag = 0;
        do
        {
            if ( avoid_hit[weight_side_index] )
            {
                blocked_weight_scale = ((1.0f - scratch_vector.n[weight_side_index]) * 2.0f);
                if ( blocked_weight_scale > 1.0f )
                    blocked_weight_scale = 1.0f;
                blocked_flag = 1;
                avoid_weight = -((avoid_ray_avoidance_weights[weight_side_index] * blocked_weight_scale) - avoid_weight);
            }
            else
            {
                clear_time_fraction = 1.0f;
                if ( !blocked_flag )
                {
                    clear_time = actor->control.vector_avoidance_clear_times[avoid_dir_index][weight_side_index];
                    if ( clear_time < 75 )  /* 2.5 s at 30 ticks/s: too recently blocked to credit */
                    {
                        clear_time_fraction = 0.0f;
                    }
                    else
                    {
                        clear_time_fraction = (1.0f - (75.0f / (float)clear_time));
                        if ( clear_time_fraction < 0.0f )
                            clear_time_fraction = 0.0f;
                        else if ( clear_time_fraction > 1.0f )
                            clear_time_fraction = 1.0f;
                    }
                }
                avoid_weight = ((avoid_ray_avoidance_weights[weight_side_index] * clear_time_fraction) + avoid_weight);
            }
            weight_side_index = (int16_t)(weight_side_index - 1);
        }
        while ( weight_side_index >= 0 );

        direction_weights[avoid_dir_index] = direction_weights[avoid_dir_index] + avoid_weight;
        weight_index_fwd1 = (avoid_dir_index + 1) % 8;
        direction_weights[weight_index_fwd1] = (avoid_weight * avoid_ray_adjacent_fractions[0]) + direction_weights[weight_index_fwd1];
        weight_index_fwd2 = (avoid_dir_index + 2) % 8;
        next_avoid_dir = (int16_t)(avoid_dir_index + 1);
        direction_weights[weight_index_fwd2] = (avoid_weight * avoid_ray_adjacent_fractions[1]) + direction_weights[weight_index_fwd2];
        weight_index_back1 = (avoid_dir_index + 7) % 8;
        weight_index_back2 = (avoid_dir_index + 6) % 8;
        avoid_dir_index = next_avoid_dir;
        direction_weights[weight_index_back1] = direction_weights[weight_index_back1] + (avoid_weight * avoid_ray_adjacent_fractions[0]);
        direction_weights[(int16_t)weight_index_back2] = (avoid_weight * avoid_ray_adjacent_fractions[1]) + direction_weights[(int16_t)weight_index_back2];
    }
    while ( next_avoid_dir < 8 );

    /* bias weights toward the object velocity direction */
    up_z = avoidance_data.up.n[2];
    up_y = avoidance_data.up.n[1];
    up_x = avoidance_data.up.n[0];
    left_z = avoidance_data.left.n[2];
    left_y = avoidance_data.left.n[1];
    left_x = avoidance_data.left.n[0];
    velocity_speed = __fsqrts(((unit_object->object.angular_velocity.n[2] * unit_object->object.angular_velocity.n[2])
                           + ((unit_object->object.angular_velocity.n[0] * unit_object->object.angular_velocity.n[0])
                                     + (unit_object->object.angular_velocity.n[1] * unit_object->object.angular_velocity.n[1]))));
    if ( velocity_speed > 0.02f )
    {
        found_direction = 0;
        velocity_bias_fraction = ((velocity_speed - 0.02f) * 12.5f);
        if ( velocity_bias_fraction > 1.0f )
            velocity_bias_fraction = 1.0f;
        velocity_bias_weight = (velocity_bias_fraction * 0.8f);
        vel_plane_x = 0.0f;
        vel_dot_up = ((unit_object->object.angular_velocity.n[0] * avoidance_data.up.n[0])
                    + ((avoidance_data.up.n[2] * unit_object->object.angular_velocity.n[2])
                              + (avoidance_data.up.n[1] * unit_object->object.angular_velocity.n[1])));
        vel_dot_left_neg = -((unit_object->object.angular_velocity.n[0] * avoidance_data.left.n[0])
                            + ((avoidance_data.left.n[2] * unit_object->object.angular_velocity.n[2])
                                      + (unit_object->object.angular_velocity.n[1] * avoidance_data.left.n[1])));
        vel_plane_mag = __fsqrts(((vel_dot_left_neg * vel_dot_left_neg) + (vel_dot_up * vel_dot_up)));
        if ( __fabs(vel_plane_mag) >= 0.000099999997f )
        {
            vel_plane_x = ((1.0f / vel_plane_mag) * 0.0f);
            scratch_vector.n[0] = (1.0f / vel_plane_mag) * 0.0f;
            vel_dot_up = ((1.0f / vel_plane_mag) * vel_dot_up);
            scratch_vector.n[1] = vel_dot_up;
            vel_dot_left_neg = ((1.0f / vel_plane_mag) * vel_dot_left_neg);
            scratch_vector.n[2] = vel_dot_left_neg;
            if ( vel_plane_mag > 0.0f )
                found_direction = actor_move_vector_avoidance_find_direction(
                                    8,
                                    avoidance_directions,
                                    direction_weights,
                                    &scratch_vector,
                                    sense_point.n,
                                    &out_fraction);
        }
        if ( found_direction && out_fraction > 0.5f )
        {
            for ( j = 0; j < 8; j = (int16_t)(j + 1) )
            {
                direction = &avoidance_directions[j];
                if ( ((direction->n[0] * vel_plane_x)
                           + ((direction->n[2] * vel_dot_left_neg) + (direction->n[1] * vel_dot_up))) < 0.0f )
                    direction_weights[j] =
                        (((direction->n[0] * vel_plane_x)
                                       + ((direction->n[2] * vel_dot_left_neg)
                                                 + (direction->n[1] * vel_dot_up)))
                                * velocity_bias_weight)
                        + direction_weights[j];
            }
        }
    }

    /* pick the best-weighted direction */
    dir_scan_index = 0;
    best_index = -1;
    current_dir_index = 0;
    best_weight = -3.4028235e38f;
    do
    {
        if ( direction_weights[dir_scan_index] > best_weight )
        {
            best_weight = direction_weights[dir_scan_index];
            best_index = current_dir_index;
        }
        current_dir_index = ++dir_scan_index;
    }
    while ( dir_scan_index < 8 );

    /* transform the desired movement_direction into the object frame */
    move_z = movement_direction->n[2];
    out_fraction = 0.0f;
    move_fit_fraction = 0.0f;
    move_x = movement_direction->n[0];
    move_dot_forward = 1.0f;
    scratch_vector.n[1] = movement_direction->n[1];
    scratch_vector.n[2] = move_z;
    move_y = scratch_vector.n[1];
    scratch_vector.n[0] = move_x;
    zero_y = global_zero_vector3d->n[1];
    zero_z = global_zero_vector3d->n[2];
    scratch_vector.n[0] = global_zero_vector3d->n[0];
    scratch_vector.n[2] = zero_z;
    move_mag_sq = ((move_x * move_x) + ((scratch_vector.n[1] * scratch_vector.n[1]) + (move_z * move_z)));
    scratch_vector.n[1] = zero_y;
    move_mag = __fsqrts(move_mag_sq);
    if ( __fabs(move_mag) < 0.000099999997f
      || (move_norm_y = (move_y * (1.0f / move_mag)),
          move_norm_z = (move_z * (1.0f / move_mag)),
          move_mag_nonpositive = move_mag <= 0.0f,
          move_norm_x = ((1.0f / move_mag) * move_x),
          move_mag_nonpositive) )
    {
        move_plane_z = scratch_vector.n[2];
        move_plane_y = scratch_vector.n[1];
        move_plane_x = scratch_vector.n[0];
    }
    else
    {
        move_plane_x = 0.0f;
        move_plane_y = ((move_norm_x * left_x)
                    + ((move_norm_z * left_z) + (move_norm_y * left_y)));
        move_plane_z = ((move_norm_x * up_x)
                    + ((move_norm_y * up_y) + (move_norm_z * up_z)));
        move_dot_forward = ((avoidance_data.forward.n[0] * move_norm_x)
                    + ((avoidance_data.forward.n[1] * move_norm_y) + (avoidance_data.forward.n[2] * move_norm_z)));
        move_plane_mag = __fsqrts(((move_plane_z * move_plane_z) + (move_plane_y * move_plane_y)));
        if ( __fabs(move_plane_mag) >= 0.000099999997f )
        {
            move_plane_x = ((1.0f / move_plane_mag) * 0.0f);
            scratch_vector.n[0] = (1.0f / move_plane_mag) * 0.0f;
            move_plane_y = ((1.0f / move_plane_mag) * move_plane_y);
            scratch_vector.n[1] = move_plane_y;
            move_plane_z = ((1.0f / move_plane_mag) * move_plane_z);
            scratch_vector.n[2] = move_plane_z;
            if ( move_plane_mag > 0.0f )
            {
                actor_move_vector_avoidance_find_direction(8, avoidance_directions,
                        direction_weights, &scratch_vector, sense_point.n, &out_fraction);
                move_fit_fraction = out_fraction;
            }
        }
    }

    weight_advantage = (best_weight - move_fit_fraction);
    is_panicking = 0;
    if ( sense_emergency <= 0.6f )
    {
        if ( (sense_emergency * 3.3333333f) >= 1.0f )
            emergency_scaled = 1.0f;
        else
            emergency_scaled = (sense_emergency * 3.3333333f);
    }
    else if ( ((sense_emergency - 0.6f) * 2.5000002f) >= 1.0f )
    {
        emergency_scaled = (1.0f + 1.0f);
    }
    else
    {
        emergency_scaled = (((sense_emergency - 0.6f) * 2.5000002f) + 1.0f);
    }

    /* decide whether to "panic" (spin away) based on forward alignment + velocity */
    if ( move_dot_forward < -0.2f )
    {
        panic_timer = actor->control.vector_avoidance_sharp_turn_timer;
        if ( panic_timer != -1 && panic_timer < 90 )
        {
            is_panicking = 1;
            goto LABEL_71;
        }
        if ( ((unit_object->object.angular_velocity.n[0] * unit_object->object.angular_velocity.n[0])
                   + ((unit_object->object.angular_velocity.n[1] * unit_object->object.angular_velocity.n[1])
                             + (unit_object->object.angular_velocity.n[2] * unit_object->object.angular_velocity.n[2]))) > 0.0025f )
        {
            if ( weight_advantage > 2.0f )
            {
                should_panic = best_weight > 2.0f;
                goto LABEL_69;
            }
            goto LABEL_70;
        }
        should_panic = emergency_scaled > 0.5f;
LABEL_69:
        is_panicking = 1;
        if ( !should_panic )
LABEL_70:
            is_panicking = 0;
    }
LABEL_71:
    panic_active = is_panicking;
    if ( is_panicking )
    {
        panic_timer_current = actor->control.vector_avoidance_sharp_turn_timer;
        if ( panic_timer_current == -1 )
            actor->control.vector_avoidance_sharp_turn_timer = 0;
        else
            actor->control.vector_avoidance_sharp_turn_timer = panic_timer_current + 1;
    }
    else
    {
        actor->control.vector_avoidance_sharp_turn_timer = -1;
    }

    if ( !panic_active )
    {
        if ( move_dot_forward >= 0.5f )
        {
            if ( sense_emergency <= 0.0f )
                goto LABEL_106;
            zero_x = global_zero_vector3d->n[0];
            nudge_zero_y = global_zero_vector3d->n[1];
            rotation_z = global_zero_vector3d->n[2];
            out_emergency = emergency_scaled;
            rotation_x = zero_x;
            rotation_y = nudge_zero_y;
            best_dir_z2 = avoidance_directions[best_index].n[2];
            best_dir_y2 = avoidance_directions[best_index].n[1];
            rot_z_acc = ((avoidance_data.up.n[2] * avoidance_directions[best_index].n[1])
                         + ((avoidance_data.left.n[2] * -best_dir_z2) + rotation_z));
            rot_y_acc = ((avoidance_data.up.n[1] * avoidance_directions[best_index].n[1])
                         + ((-best_dir_z2 * avoidance_data.left.n[1]) + nudge_zero_y));
            rotation_y = (avoidance_data.up.n[1] * avoidance_directions[best_index].n[1])
                       + ((-best_dir_z2 * avoidance_data.left.n[1]) + nudge_zero_y);
            rotation_z = rot_z_acc;
            rotation_x = (best_dir_y2 * avoidance_data.up.n[0]) + ((-best_dir_z2 * avoidance_data.left.n[0]) + zero_x);
            rot_mag = __fsqrts(((rotation_x * rotation_x)
                                    + ((rot_z_acc * rot_z_acc) + (rot_y_acc * rot_y_acc))));
            if ( __fabs(rot_mag) < 0.000099999997f )
                goto LABEL_105;
            rotation_x = (1.0f / rot_mag)
                       * ((best_dir_y2 * avoidance_data.up.n[0]) + ((-best_dir_z2 * avoidance_data.left.n[0]) + zero_x));
            rot_y_norm = (rot_y_acc * (1.0f / rot_mag));
            rotation_y = rot_y_norm;
            rot_z_norm = (rot_z_acc * (1.0f / rot_mag));
            rotation_z = rot_z_norm;
            if ( rot_mag <= 0.0f )
                goto LABEL_105;
            rotation_x = ((1.0f / rot_mag)
                                 * ((best_dir_y2 * avoidance_data.up.n[0]) + ((-best_dir_z2 * avoidance_data.left.n[0]) + zero_x)))
                       * (emergency_scaled * 1.0471976f);   /* 60 deg in radians */
            rotation_y_comp = (rot_y_norm * (emergency_scaled * 1.0471976f));
            rotation_z_comp = (rot_z_norm * (emergency_scaled * 1.0471976f));
LABEL_104:
            rotation_z = rotation_z_comp;
            rotation_y = rotation_y_comp;
            goto LABEL_105;
        }

        if ( weight_advantage <= 1.3f )
            goto LABEL_106;
        slide_direction = &avoidance_directions[best_index];
        if ( ((slide_direction->n[2] * move_plane_z)
                   + ((slide_direction->n[1] * move_plane_y) + (slide_direction->n[0] * move_plane_x))) <= 0.5f )
            goto LABEL_106;
        slide_emergency = (((best_weight - move_fit_fraction) * 0.76923078f) - 0.5f);
        out_emergency = ((best_weight - move_fit_fraction) * 0.76923078f) - 0.5f;
        if ( slide_emergency >= 0.0f )
        {
            if ( slide_emergency <= 1.0f )
            {
LABEL_95:
                if ( slide_emergency <= emergency_scaled )
                {
                    out_emergency = emergency_scaled;
                    slide_emergency = emergency_scaled;
                }
                slide_angle = (slide_emergency * 1.0471976f);   /* 60 deg in radians */
                if ( ((slide_direction->n[1] * move_plane_z) - (slide_direction->n[2] * move_plane_y)) > 0.0f )
                    slide_angle = -slide_angle;
                rotation_y_comp = (avoidance_data.forward.n[1] * slide_angle);
                rotation_x = avoidance_data.forward.n[0] * slide_angle;
                rotation_z_comp = (avoidance_data.forward.n[2] * slide_angle);
                goto LABEL_104;
            }
            out_emergency = 1.0f;
        }
        else
        {
            out_emergency = 0.0f;
        }
        slide_emergency = out_emergency;
        goto LABEL_95;
    }

    /* panic path: rotate directly toward the chosen avoidance direction */
    panic_zero_y = global_zero_vector3d->n[1];
    panic_zero_z = global_zero_vector3d->n[2];
    world_direction.n[0] = global_zero_vector3d->n[0];
    best_direction = &avoidance_directions[best_index];
    world_direction.n[1] = panic_zero_y;
    best_dir_x = best_direction->n[0];
    world_z_acc = ((avoidance_data.forward.n[2] * best_direction->n[0]) + panic_zero_z);
    world_direction.n[2] = panic_zero_z;
    best_dir_y = best_direction->n[1];
    world_x_acc = ((best_dir_x * avoidance_data.forward.n[0]) + world_direction.n[0]);
    world_z_acc2 = ((avoidance_data.left.n[2] * best_direction->n[1]) + world_z_acc);
    best_dir_z = best_direction->n[2];
    world_y_acc = ((avoidance_data.forward.n[1] * best_dir_x) + panic_zero_y);
    move_dir_x = movement_direction->n[0];
    move_dir_y = movement_direction->n[1];
    move_dir_z = movement_direction->n[2];
    world_direction.n[2] = (avoidance_data.up.n[2] * best_direction->n[2]) + world_z_acc2;
    world_direction.n[0] = (best_dir_z * avoidance_data.up.n[0]) + ((best_dir_y * avoidance_data.left.n[0]) + world_x_acc);
    world_direction.n[1] = (avoidance_data.up.n[1] * best_dir_z) + ((best_dir_y * avoidance_data.left.n[1]) + world_y_acc);
    cross_y = ((move_dir_z * world_direction.n[0]) - (move_dir_x * world_direction.n[2]));
    cross_z = ((move_dir_x * world_direction.n[1]) - (move_dir_y * world_direction.n[0]));
    cross_x = ((move_dir_y * world_direction.n[2]) - (move_dir_z * world_direction.n[1]));
    cross_mag = __fsqrts(((cross_x * cross_x)
                            + ((cross_z * cross_z) + (cross_y * cross_y))));
    if ( __fabs(cross_mag) >= 0.000099999997f )
    {
        axis_x = ((1.0f / cross_mag) * cross_x);
        axis_y = (((move_dir_z * world_direction.n[0]) - (move_dir_x * world_direction.n[2]))
                     * (1.0f / cross_mag));
        axis_z = (cross_z * (1.0f / cross_mag));
        if ( cross_mag > 0.0f )
        {
            rotation_angle = angle_between_vectors3d(movement_direction, &world_direction);
            rotation_x = axis_x * rotation_angle;
            rotation_y = axis_y * rotation_angle;
            rotation_z = axis_z * rotation_angle;
        }
    }
    panic_emergency = (((2.0f - move_fit_fraction) * 0.5f) - 0.5f);
    if ( panic_emergency >= 0.0f )
    {
        if ( panic_emergency <= 1.0f )
            out_emergency = ((2.0f - move_fit_fraction) * 0.5f) - 0.5f;
        else
            out_emergency = 1.0f;
    }
    else
    {
        out_emergency = 0.0f;
    }
    if ( out_emergency <= emergency_scaled )
        out_emergency = emergency_scaled;

LABEL_105:
    result_written = 1;
LABEL_106:
    if ( result_written )
        actor->control.vector_avoidance_current_direction = best_index;
    else
        actor->control.vector_avoidance_current_direction = -1;

LABEL_109:
    final_rotation_y = rotation_y;
    final_rotation_z = rotation_z;
    avoidance_rotation->n[0] = rotation_x;
    avoidance_rotation->n[1] = final_rotation_y;
    avoidance_rotation->n[2] = final_rotation_z;
    *emergency_amount = out_emergency;
}
