/*
 * actor_move_vector_avoidance @ 0x837C93B8  (size 3312)
 *
 * CAVEAT (reg-alloc / 64-bit register smearing):
 *   The decompiler flagged "positive sp value has been detected" (this frame is
 *   ~0x61E0 bytes).  Two forms of packing were untangled here:
 *     - The parameter/flag values the decompiler smeared into a single __int64
 *       have been recovered as the named parameters (movement_direction et al.),
 *       the zero-initialised direction_weights[8], and the result_written flag.
 *     - clear_time_packed is a genuine 64-bit slot the schedule reuses for two
 *       independent values: its high dword (HIDWORD) is the byte offset into the
 *       2-word out_fraction result buffer, and its low dword (LODWORD) is the
 *       per-side clear-time byte read from actor->control.vector_avoidance_clear_times[avoid_dir_index][side].  The
 *       HIDWORD/LODWORD split is preserved verbatim because the two halves are
 *       genuinely independent in the original schedule.
 *     - The single-slot store `*(__int64*)avoid_point.n = clear_time_packed` is a
 *       decompiler artifact of that packing; the meaningful use is the float
 *       divide `75.0 / clear_time_value` immediately after.
 *   RtlCheckStack12() is the compiler stack-probe for the large frame and is
 *   dropped; the datum index comes directly from actor_index.  Every scratch
 *   temporary has been given a descriptive name derived from the arithmetic it
 *   performs or the value it feeds; no decompiler-generated names remain.
 */

#include <stdint.h>
#include "headers/hexrays_defs.h" /* LODWORD/HIDWORD word-extraction macros */
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
#include "headers/vector_avoidance_ray.h"
extern float __fsqrts(float);

/* --- extern boundaries (siblings / globals) --------------------------------- */

extern float sense_ray_avoidance_weights[9][8];
extern float avoid_ray_avoidance_weights[2];

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
    unsigned char result_written;

    double sense_emergency;
    double sense_clear_fraction, sense_weight_scale, sense_weight_clamped;
    int weight_index_next, weight_index_back, weight_index;
    int i, j;
    __int16 last_direction;

    /* left = forward x up cross product, seeded into avoidance_data.left */
    double left_cross_x, left_cross_y, left_cross_z;

    int avoid_dir_index, side_index, clear_time_base, next_side_index, weight_side_index;
    double avoid_weight, clear_time_fraction, blocked_weight_scale;
    char blocked_flag;
    __int64 clear_time_packed;               /* HIDWORD = result-word byte offset, LODWORD = clear-time value */
    int weight_index_fwd1, weight_index_fwd2, next_avoid_dir, weight_index_back1, weight_index_back2;

    /* object up/left basis reloaded for the velocity-bias + move transform */
    double up_x, up_y, up_z, left_x, left_y, left_z;
    float velocity_speed;
    unsigned char found_direction;
    double velocity_bias_fraction, velocity_bias_weight;
    double vel_plane_x, vel_dot_up, vel_dot_left_neg, vel_plane_mag;
    real_vector3d *direction;

    __int16 best_index;
    __int16 dir_scan_index, current_dir_index;
    double best_weight;
    float move_z, move_x, zero_y, zero_z;
    double move_fit_fraction, move_dot_forward, move_y, move_mag_sq, move_mag;
    double move_norm_x, move_norm_y, move_norm_z;
    bool move_mag_nonpositive;
    double move_plane_x, move_plane_y, move_plane_z, move_plane_mag, weight_advantage;
    unsigned char is_panicking;
    double emergency_scaled;
    int panic_timer;
    bool should_panic;
    int panic_active, panic_timer_current;
    float panic_zero_y, panic_zero_z;
    real_vector3d *best_direction;
    double best_dir_x, world_z_acc, best_dir_y, world_x_acc, world_z_acc2, best_dir_z;
    double world_y_acc, move_dir_x, move_dir_y, move_dir_z;
    double cross_y, cross_z, cross_x, cross_mag;
    double axis_x, axis_y, axis_z, rotation_angle, panic_emergency;
    real_vector3d *slide_direction;
    double slide_emergency, slide_angle, rotation_y_comp, rotation_z_comp;
    float zero_x, nudge_zero_y;
    double best_dir_z2, best_dir_y2, rot_z_acc, rot_y_acc, rot_mag, rot_y_norm, rot_z_norm;
    float final_rotation_y, final_rotation_z;

    float out_emergency;                 /* -> *emergency_amount */
    float out_fraction;                  /* also reused as a 2-word scratch buffer */
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

    unsigned __int16 actor_datum_index = (unsigned __int16)actor_index;

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

    sense_emergency = 0.0;
    /* zero-initialise the 8 direction weights */
    direction_weights[0] = 0.0f; direction_weights[1] = 0.0f;
    direction_weights[2] = 0.0f; direction_weights[3] = 0.0f;
    direction_weights[4] = 0.0f; direction_weights[5] = 0.0f;
    direction_weights[6] = 0.0f; direction_weights[7] = 0.0f;

    /* seed weights biased around the last chosen direction */
    last_direction = actor->control.vector_avoidance_current_direction;
    if ( (unsigned int)last_direction < 8 )
    {
        weight_index_next = (last_direction + 1) % 8;
        *(direction_weights + last_direction) = *(direction_weights + last_direction) + (float)0.40000001;
        direction_weights[weight_index_next] = direction_weights[weight_index_next] + (float)0.32000002;
        *(direction_weights + (last_direction + 2) % 8) =
            (float)(0.5 * (float)0.40000001) + *(direction_weights + (last_direction + 2) % 8);
        *(direction_weights + (last_direction + 7) % 8) =
            *(direction_weights + (last_direction + 7) % 8) + (float)0.32000002;
        weight_index_back = (last_direction + 6) % 8;
        direction_weights[weight_index_back] =
            (float)(0.5 * (float)0.40000001) + direction_weights[weight_index_back];
    }

    /* sense rays: accumulate emergency + directional weights */
    for ( i = 0; i < 9; i = (__int16)(i + 1) )
    {
        if ( actor_move_test_avoidance_vector(&avoidance_data, &sense_rays[i],
                                              &sense_point, &scratch_vector, &out_fraction,
                                              0) > 0 )
        {
            sense_clear_fraction = ((float)1.0 - out_fraction);
            weight_index = 0;
            sense_weight_scale = (((float)1.0 - out_fraction) * (float)2.0);
            do
            {
                sense_weight_clamped = sense_weight_scale;
                if ( sense_weight_scale > 1.0 )
                    sense_weight_clamped = 1.0;
                *(direction_weights + weight_index) =
                    (sense_ray_avoidance_weights[i][weight_index] * (float)sense_weight_clamped)
                    + *(direction_weights + weight_index);
                weight_index = (__int16)(weight_index + 1);
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
            clear_time_base = 2 * (avoid_dir_index + 740);
            /* out_fraction is reused here as a 2-word result buffer */
            *((__int16 *)&out_fraction + side_index) = actor_move_test_avoidance_vector(
                    &avoidance_data,
                    &avoidance_rays[avoid_dir_index][side_index],
                    &avoid_point,
                    &avoid_result_vector,
                    &scratch_vector.n[side_index],
                    &actor->control.vector_avoidance_clear_times[avoid_dir_index][side_index]);
            next_side_index = (__int16)(side_index + 1);
            side_index = next_side_index;
        }
        while ( next_side_index < 2 );

        weight_side_index = 1;
        avoid_weight = 0.0;
        blocked_flag = 0;
        *(int *)&clear_time_packed = 2; /* BE: high dword = bytes 0-3 */
        do
        {
            if ( *(__int16 *)((char *)&out_fraction + *(int *)&clear_time_packed) )
            {
                blocked_weight_scale = (((float)1.0 - scratch_vector.n[weight_side_index]) * (float)2.0);
                if ( blocked_weight_scale > 1.0 )
                    blocked_weight_scale = 1.0;
                blocked_flag = 1;
                avoid_weight = -((avoid_ray_avoidance_weights[weight_side_index] * (float)blocked_weight_scale) - (float)avoid_weight);
            }
            else
            {
                clear_time_fraction = 1.0;
                if ( !blocked_flag )
                {
                    *((int *)&clear_time_packed + 1) = actor->control.vector_avoidance_clear_times[avoid_dir_index][weight_side_index]; /* BE: low dword = bytes 4-7 */
                    if ( (unsigned int)clear_time_packed < 0x4B )
                    {
                        avoid_weight = ((avoid_ray_avoidance_weights[weight_side_index] * (float)0.0) + (float)avoid_weight);
                        goto LABEL_30;
                    }
                    *(__int64 *)avoid_point.n = clear_time_packed;
                    clear_time_fraction = ((float)1.0 - ((float)75.0 / (float)clear_time_packed));
                    if ( clear_time_fraction < 0.0 )
                    {
                        avoid_weight = ((avoid_ray_avoidance_weights[weight_side_index] * (float)0.0) + (float)avoid_weight);
                        goto LABEL_30;
                    }
                    if ( clear_time_fraction > 1.0 )
                        clear_time_fraction = 1.0;
                }
                avoid_weight = ((avoid_ray_avoidance_weights[weight_side_index] * (float)clear_time_fraction) + (float)avoid_weight);
            }
LABEL_30:
            weight_side_index = (__int16)(weight_side_index - 1);
            *(int *)&clear_time_packed = 2 * weight_side_index; /* BE: high dword = bytes 0-3 */
        }
        while ( weight_side_index >= 0 );

        *(direction_weights + avoid_dir_index) = *(direction_weights + avoid_dir_index) + (float)avoid_weight;
        weight_index_fwd1 = (avoid_dir_index + 1) % 8;
        direction_weights[weight_index_fwd1] = (float)((float)avoid_weight * 0.80000001) + direction_weights[weight_index_fwd1];
        weight_index_fwd2 = (avoid_dir_index + 2) % 8;
        next_avoid_dir = (__int16)(avoid_dir_index + 1);
        direction_weights[weight_index_fwd2] = (float)((float)avoid_weight * 0.5) + direction_weights[weight_index_fwd2];
        weight_index_back1 = (avoid_dir_index + 7) % 8;
        weight_index_back2 = (avoid_dir_index + 6) % 8;
        avoid_dir_index = next_avoid_dir;
        direction_weights[weight_index_back1] = direction_weights[weight_index_back1] + (float)((float)avoid_weight * 0.80000001);
        *(direction_weights + (__int16)weight_index_back2) = (float)((float)avoid_weight * 0.5) + *(direction_weights + (__int16)weight_index_back2);
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
    if ( velocity_speed > 0.02 )
    {
        found_direction = 0;
        velocity_bias_fraction = ((velocity_speed - (float)0.02) * (float)12.5);
        if ( velocity_bias_fraction > 1.0 )
            velocity_bias_fraction = 1.0;
        velocity_bias_weight = ((float)velocity_bias_fraction * (float)0.80000001);
        vel_plane_x = 0.0;
        vel_dot_up = ((unit_object->object.angular_velocity.n[0] * avoidance_data.up.n[0])
                    + ((avoidance_data.up.n[2] * unit_object->object.angular_velocity.n[2])
                              + (avoidance_data.up.n[1] * unit_object->object.angular_velocity.n[1])));
        vel_dot_left_neg = -((unit_object->object.angular_velocity.n[0] * avoidance_data.left.n[0])
                            + ((avoidance_data.left.n[2] * unit_object->object.angular_velocity.n[2])
                                      + (unit_object->object.angular_velocity.n[1] * avoidance_data.left.n[1])));
        vel_plane_mag = __fsqrts((((float)vel_dot_left_neg * (float)vel_dot_left_neg) + ((float)vel_dot_up * (float)vel_dot_up)));
        if ( __fabs(vel_plane_mag) >= 0.000099999997 )
        {
            vel_plane_x = (((float)1.0 / (float)vel_plane_mag) * (float)0.0);
            scratch_vector.n[0] = ((float)1.0 / (float)vel_plane_mag) * (float)0.0;
            vel_dot_up = (((float)1.0 / (float)vel_plane_mag) * (float)vel_dot_up);
            scratch_vector.n[1] = vel_dot_up;
            vel_dot_left_neg = (((float)1.0 / (float)vel_plane_mag) * (float)vel_dot_left_neg);
            scratch_vector.n[2] = vel_dot_left_neg;
            if ( vel_plane_mag > 0.0 )
                found_direction = actor_move_vector_avoidance_find_direction(
                                    8,
                                    avoidance_directions,
                                    direction_weights,
                                    &scratch_vector,
                                    sense_point.n,
                                    &out_fraction);
        }
        if ( found_direction && out_fraction > 0.5 )
        {
            for ( j = 0; j < 8; j = (__int16)(j + 1) )
            {
                direction = &avoidance_directions[j];
                if ( ((direction->n[0] * (float)vel_plane_x)
                           + ((direction->n[2] * (float)vel_dot_left_neg) + (direction->n[1] * (float)vel_dot_up))) < 0.0 )
                    *(direction_weights + j) =
                        (((direction->n[0] * (float)vel_plane_x)
                                       + ((direction->n[2] * (float)vel_dot_left_neg)
                                                 + (direction->n[1] * (float)vel_dot_up)))
                                * (float)velocity_bias_weight)
                        + *(direction_weights + j);
            }
        }
    }

    /* pick the best-weighted direction */
    dir_scan_index = 0;
    best_index = -1;
    current_dir_index = 0;
    best_weight = -3.4028235e38;
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
    move_fit_fraction = 0.0;
    move_x = movement_direction->n[0];
    move_dot_forward = 1.0;
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
    if ( __fabs(move_mag) < 0.000099999997
      || (move_norm_y = ((float)move_y * ((float)1.0 / (float)move_mag)),
          move_norm_z = (move_z * ((float)1.0 / (float)move_mag)),
          move_mag_nonpositive = move_mag <= 0.0,
          move_norm_x = (((float)1.0 / (float)move_mag) * move_x),
          move_mag_nonpositive) )
    {
        move_plane_z = scratch_vector.n[2];
        move_plane_y = scratch_vector.n[1];
        move_plane_x = scratch_vector.n[0];
    }
    else
    {
        move_plane_x = 0.0;
        move_plane_y = (((float)move_norm_x * (float)left_x)
                    + (((float)move_norm_z * (float)left_z) + ((float)move_norm_y * (float)left_y)));
        move_plane_z = (((float)move_norm_x * (float)up_x)
                    + (((float)move_norm_y * (float)up_y) + ((float)move_norm_z * (float)up_z)));
        move_dot_forward = ((avoidance_data.forward.n[0] * (float)move_norm_x)
                    + ((avoidance_data.forward.n[1] * (float)move_norm_y) + (avoidance_data.forward.n[2] * (float)move_norm_z)));
        move_plane_mag = __fsqrts((((float)move_plane_z * (float)move_plane_z) + ((float)move_plane_y * (float)move_plane_y)));
        if ( __fabs(move_plane_mag) >= 0.000099999997 )
        {
            move_plane_x = (((float)1.0 / (float)move_plane_mag) * (float)0.0);
            scratch_vector.n[0] = ((float)1.0 / (float)move_plane_mag) * (float)0.0;
            move_plane_y = (((float)1.0 / (float)move_plane_mag) * (float)move_plane_y);
            scratch_vector.n[1] = move_plane_y;
            move_plane_z = (((float)1.0 / (float)move_plane_mag) * (float)move_plane_z);
            scratch_vector.n[2] = move_plane_z;
            if ( move_plane_mag > 0.0 )
            {
                actor_move_vector_avoidance_find_direction(8, avoidance_directions,
                        direction_weights, &scratch_vector, sense_point.n, &out_fraction);
                move_fit_fraction = out_fraction;
            }
        }
    }

    weight_advantage = ((float)best_weight - (float)move_fit_fraction);
    is_panicking = 0;
    if ( sense_emergency <= 0.60000002 )
    {
        if ( ((float)sense_emergency * (float)3.3333333) >= 1.0 )
            emergency_scaled = 1.0;
        else
            emergency_scaled = ((float)sense_emergency * (float)3.3333333);
    }
    else if ( (((float)sense_emergency - (float)0.60000002) * (float)2.5000002) >= 1.0 )
    {
        emergency_scaled = ((float)1.0 + (float)1.0);
    }
    else
    {
        emergency_scaled = ((((float)sense_emergency - (float)0.60000002) * (float)2.5000002) + (float)1.0);
    }

    /* decide whether to "panic" (spin away) based on forward alignment + velocity */
    if ( move_dot_forward < -0.2 )
    {
        panic_timer = actor->control.vector_avoidance_sharp_turn_timer;
        if ( panic_timer != -1 && panic_timer < 90 )
        {
            is_panicking = 1;
            goto LABEL_71;
        }
        if ( ((unit_object->object.angular_velocity.n[0] * unit_object->object.angular_velocity.n[0])
                   + ((unit_object->object.angular_velocity.n[1] * unit_object->object.angular_velocity.n[1])
                             + (unit_object->object.angular_velocity.n[2] * unit_object->object.angular_velocity.n[2]))) > 0.0025000002 )
        {
            if ( weight_advantage > 2.0 )
            {
                should_panic = best_weight > 2.0;
                goto LABEL_69;
            }
            goto LABEL_70;
        }
        should_panic = emergency_scaled > 0.5;
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
        if ( move_dot_forward >= 0.5 )
        {
            if ( sense_emergency <= 0.0 )
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
                         + ((avoidance_data.left.n[2] * (float)-best_dir_z2) + rotation_z));
            rot_y_acc = ((avoidance_data.up.n[1] * avoidance_directions[best_index].n[1])
                         + (((float)-best_dir_z2 * avoidance_data.left.n[1]) + nudge_zero_y));
            rotation_y = (avoidance_data.up.n[1] * avoidance_directions[best_index].n[1])
                       + (((float)-best_dir_z2 * avoidance_data.left.n[1]) + nudge_zero_y);
            rotation_z = rot_z_acc;
            rotation_x = ((float)best_dir_y2 * avoidance_data.up.n[0]) + (((float)-best_dir_z2 * avoidance_data.left.n[0]) + zero_x);
            rot_mag = __fsqrts(((rotation_x * rotation_x)
                                    + (((float)rot_z_acc * (float)rot_z_acc) + ((float)rot_y_acc * (float)rot_y_acc))));
            if ( __fabs(rot_mag) < 0.000099999997 )
                goto LABEL_105;
            rotation_x = ((float)1.0 / (float)rot_mag)
                       * (((float)best_dir_y2 * avoidance_data.up.n[0]) + (((float)-best_dir_z2 * avoidance_data.left.n[0]) + zero_x));
            rot_y_norm = ((float)rot_y_acc * ((float)1.0 / (float)rot_mag));
            rotation_y = rot_y_norm;
            rot_z_norm = ((float)rot_z_acc * ((float)1.0 / (float)rot_mag));
            rotation_z = rot_z_norm;
            if ( rot_mag <= 0.0 )
                goto LABEL_105;
            rotation_x = (((float)1.0 / (float)rot_mag)
                                 * (((float)best_dir_y2 * avoidance_data.up.n[0]) + (((float)-best_dir_z2 * avoidance_data.left.n[0]) + zero_x)))
                       * ((float)emergency_scaled * (float)1.0471976);
            rotation_y_comp = ((float)rot_y_norm * ((float)emergency_scaled * (float)1.0471976));
            rotation_z_comp = ((float)rot_z_norm * ((float)emergency_scaled * (float)1.0471976));
LABEL_104:
            rotation_z = rotation_z_comp;
            rotation_y = rotation_y_comp;
            goto LABEL_105;
        }

        if ( weight_advantage <= 1.3 )
            goto LABEL_106;
        slide_direction = &avoidance_directions[best_index];
        if ( ((slide_direction->n[2] * (float)move_plane_z)
                   + ((slide_direction->n[1] * (float)move_plane_y) + (slide_direction->n[0] * (float)move_plane_x))) <= 0.5 )
            goto LABEL_106;
        slide_emergency = ((((float)best_weight - (float)move_fit_fraction) * (float)0.76923078) - (float)0.5);
        out_emergency = (((float)best_weight - (float)move_fit_fraction) * (float)0.76923078) - (float)0.5;
        if ( slide_emergency >= 0.0 )
        {
            if ( slide_emergency <= 1.0 )
            {
LABEL_95:
                if ( slide_emergency <= emergency_scaled )
                {
                    out_emergency = emergency_scaled;
                    slide_emergency = emergency_scaled;
                }
                slide_angle = ((float)slide_emergency * (float)1.0471976);
                if ( ((slide_direction->n[1] * (float)move_plane_z) - (slide_direction->n[2] * (float)move_plane_y)) > 0.0 )
                    slide_angle = -slide_angle;
                rotation_y_comp = (avoidance_data.forward.n[1] * (float)slide_angle);
                rotation_x = avoidance_data.forward.n[0] * (float)slide_angle;
                rotation_z_comp = (avoidance_data.forward.n[2] * (float)slide_angle);
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
    world_x_acc = (((float)best_dir_x * avoidance_data.forward.n[0]) + world_direction.n[0]);
    world_z_acc2 = ((avoidance_data.left.n[2] * best_direction->n[1]) + (float)world_z_acc);
    best_dir_z = best_direction->n[2];
    world_y_acc = ((avoidance_data.forward.n[1] * (float)best_dir_x) + panic_zero_y);
    move_dir_x = movement_direction->n[0];
    move_dir_y = movement_direction->n[1];
    move_dir_z = movement_direction->n[2];
    world_direction.n[2] = (avoidance_data.up.n[2] * best_direction->n[2]) + (float)world_z_acc2;
    world_direction.n[0] = ((float)best_dir_z * avoidance_data.up.n[0]) + (((float)best_dir_y * avoidance_data.left.n[0]) + (float)world_x_acc);
    world_direction.n[1] = (avoidance_data.up.n[1] * (float)best_dir_z) + (((float)best_dir_y * avoidance_data.left.n[1]) + (float)world_y_acc);
    cross_y = (((float)move_dir_z * world_direction.n[0]) - ((float)move_dir_x * world_direction.n[2]));
    cross_z = (((float)move_dir_x * world_direction.n[1]) - ((float)move_dir_y * world_direction.n[0]));
    cross_x = (((float)move_dir_y * world_direction.n[2]) - ((float)move_dir_z * world_direction.n[1]));
    cross_mag = __fsqrts((((float)cross_x * (float)cross_x)
                            + (((float)cross_z * (float)cross_z) + ((float)cross_y * (float)cross_y))));
    if ( __fabs(cross_mag) >= 0.000099999997 )
    {
        axis_x = (((float)1.0 / (float)cross_mag) * (float)cross_x);
        axis_y = ((((float)move_dir_z * world_direction.n[0]) - ((float)move_dir_x * world_direction.n[2]))
                     * ((float)1.0 / (float)cross_mag));
        axis_z = ((float)cross_z * ((float)1.0 / (float)cross_mag));
        if ( cross_mag > 0.0 )
        {
            rotation_angle = angle_between_vectors3d(movement_direction, &world_direction);
            rotation_x = (float)axis_x * (float)rotation_angle;
            rotation_y = (float)axis_y * (float)rotation_angle;
            rotation_z = (float)axis_z * (float)rotation_angle;
        }
    }
    panic_emergency = ((((float)2.0 - (float)move_fit_fraction) * (float)0.5) - (float)0.5);
    if ( panic_emergency >= 0.0 )
    {
        if ( panic_emergency <= 1.0 )
            out_emergency = (((float)2.0 - (float)move_fit_fraction) * (float)0.5) - (float)0.5;
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
