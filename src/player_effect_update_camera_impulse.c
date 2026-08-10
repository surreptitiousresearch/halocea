/* player_effect_update_camera_impulse @0x83738098 — fold a camera-impulse definition (a directional view
 * kick) into a local player's running player-effect state. The "temporary" part is a decaying impulse: it is
 * (re)applied only when the new scaled strength dominates the current one, in which case a fresh impulse
 * direction is built in the camera plane from the requested world direction and the player's facing, a random
 * jitter axis is generated, and the camera_impulse_time_left countdown is reset. The "permanent" part is a
 * sustained lean applied every call: a yaw from the triple product of (direction, up, facing) and a pitch from
 * dot(direction, facing), scaled by permanent_angle and the permanent zero-scale blend, handed to
 * player_control_permanent_impulse.
 *
 * `scale` and `time_scale` are floats (DB prototype); the decompiler widened them to double (FPR args). All
 * the nested (float)(...) casts are single-precision intermediates and are preserved. cos/sin return double
 * and are narrowed to float on store. (int)camera_impulse_time_left -> float is an fcfid artifact. */

#include <stdint.h>
#include <string.h>
#include "headers/real_vector2d.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/camera_impulse_definition.h"
#include "headers/player_effect_datum.h"
#include "headers/screen_flash_just_started_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"
extern double cos(double x);
extern double sin(double x);


extern int game_time_get(void);
extern const real_euler_angles2d *player_control_get_facing_angles(int16_t local_player_index);
extern real_vector3d *player_control_get_facing_direction(int16_t local_player_index, real_vector3d *direction);
extern void player_control_permanent_impulse(int16_t local_player_index, const real_euler_angles2d *delta);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern float signed_angle_between_vectors2d(const real_vector2d *a, const real_vector2d *b);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern float normalize3d(real_vector3d *v);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern float __fsqrts(float x);
extern double __fabs(double x);

void player_effect_update_camera_impulse(int16_t local_player_index, player_effect_datum *datum,
                                         const camera_impulse_definition *camera_impulse,
                                         const real_vector3d *direction, float scale, float time_scale)
{
    game_time_get();

    float time_left = (float)(int)datum->camera_impulse_time_left;
    float current_duration = datum->camera_impulse.temporary_duration;
    camera_impulse_definition *p_camera_impulse = &datum->camera_impulse;

    float new_zero_scale = (1.0f - camera_impulse->temporary_zero_scale_factor) * scale
                           + camera_impulse->temporary_zero_scale_factor;
    float current_zero_scale = datum->camera_impulse.temporary_zero_scale_factor;

    if (current_duration > time_left
        || new_zero_scale > current_zero_scale
        || (new_zero_scale >= current_zero_scale
            && time_left < camera_impulse->temporary_duration * (time_scale * 30.0f)))
    {
        /* normalize the requested world direction in the camera (xy) plane */
        real_vector2d impulse_direction;
        float impulse_direction_z = 0.0f;
        impulse_direction.n[0] = direction->n[0];
        impulse_direction.n[1] = direction->n[1];
        float direction_length = __fsqrts(impulse_direction.n[1] * impulse_direction.n[1]
                                          + impulse_direction.n[0] * impulse_direction.n[0]);
        if (__fabs(direction_length) >= 0.000099999997f)
        {
            float inverse = 1.0f / direction_length;
            impulse_direction.n[0] = inverse * impulse_direction.n[0];
            impulse_direction.n[1] = impulse_direction.n[1] * inverse;
            impulse_direction_z = inverse * 0.0f;
        }

        /* the player's facing direction, projected to the camera plane and normalized */
        real_euler_angles2d facing_angles = *player_control_get_facing_angles(local_player_index);
        real_vector3d facing;
        vector3d_from_euler_angles2d(&facing, &facing_angles);
        float facing_x = facing.n[0];
        float facing_y = facing.n[1];
        float facing_z = 0.0f;
        facing.n[2] = 0.0f;
        float facing_length = __fsqrts(facing.n[1] * facing.n[1] + facing.n[0] * facing.n[0]);
        if (__fabs(facing_length) >= 0.000099999997f)
        {
            float inverse = 1.0f / facing_length;
            facing_x = inverse * facing.n[0];
            facing.n[0] = inverse * facing.n[0];
            facing_y = facing.n[1] * inverse;
            facing.n[1] = facing.n[1] * inverse;
            facing_z = inverse * 0.0f;
            facing.n[2] = inverse * 0.0f;
        }

        /* only proceed when both planar vectors are unit length */
        if (__fabs((impulse_direction_z * impulse_direction_z
                    + (impulse_direction.n[1] * impulse_direction.n[1]
                       + impulse_direction.n[0] * impulse_direction.n[0])) - 1.0f) < 0.000099999997f
            && __fabs((facing_z * facing_z + (facing_y * facing_y + facing_x * facing_x)) - 1.0f)
                   < 0.000099999997f)
        {
            float angle = signed_angle_between_vectors2d((const real_vector2d *)&facing, &impulse_direction);
            memcpy(&datum->camera_impulse, camera_impulse, sizeof(datum->camera_impulse));

            float scaled_duration = (time_scale * 30.0f) * p_camera_impulse->temporary_duration;
            datum->camera_impulse.temporary_zero_scale_factor = new_zero_scale;
            p_camera_impulse->temporary_duration = scaled_duration;
            datum->camera_impulse_time_left = (int)scaled_duration;

            datum->direction.n[0] = (float)cos((double)angle);
            datum->direction.n[1] = (float)sin((double)angle);
            datum->direction.n[2] = 0.0f;

            float jitter_upper = datum->camera_impulse.temporary_jitter_upper_bound;
            float jitter_lower = datum->camera_impulse.temporary_jitter_lower_bound;
            float jitter_magnitude =
                real_seed_random_range(get_global_local_random_seed_address(), jitter_lower, jitter_upper);
            float jitter_rotation =
                real_seed_random_range(get_global_local_random_seed_address(), 0.0f, TWO_PI);

            /* jitter axis = direction x up, normalized */
            datum->jitter.n[0] = global_up3d->n[2] * datum->direction.n[1]
                                 - global_up3d->n[1] * datum->direction.n[2];
            datum->jitter.n[2] = global_up3d->n[1] * datum->direction.n[0]
                                 - global_up3d->n[0] * datum->direction.n[1];
            datum->jitter.n[1] = global_up3d->n[0] * datum->direction.n[2]
                                 - global_up3d->n[2] * datum->direction.n[0];
            normalize3d(&datum->jitter);
            rotate_vector_about_axis(&datum->jitter, &datum->direction,
                                     (float)sin((double)jitter_rotation), (float)cos((double)jitter_rotation));
            datum->jitter.n[0] = datum->jitter.n[0] * jitter_magnitude;
            datum->jitter.n[1] = datum->jitter.n[1] * jitter_magnitude;
            datum->jitter.n[2] = datum->jitter.n[2] * jitter_magnitude;
            datum->flags |= (1u << _camera_impulse_just_started_bit);
        }
    }

    /* sustained "permanent" lean, applied every call */
    float permanent_blend = (1.0f - camera_impulse->permanent_zero_scale_factor) * scale
                            + camera_impulse->permanent_zero_scale_factor;
    real_vector3d facing_direction;
    player_control_get_facing_direction(local_player_index, &facing_direction);

    real_euler_angles2d permanent_delta;
    permanent_delta.yaw =
        ((direction->n[2] * (global_up3d->n[0] * facing_direction.n[1] - global_up3d->n[1] * facing_direction.n[0]))
         + (direction->n[0] * (global_up3d->n[1] * facing_direction.n[2] - global_up3d->n[2] * facing_direction.n[1]))
         + (direction->n[1] * (global_up3d->n[2] * facing_direction.n[0] - global_up3d->n[0] * facing_direction.n[2])))
        * camera_impulse->permanent_angle * permanent_blend;
    permanent_delta.pitch =
        ((direction->n[2] * facing_direction.n[2])
         + (direction->n[0] * facing_direction.n[0] + direction->n[1] * facing_direction.n[1]))
        * camera_impulse->permanent_angle * permanent_blend;

    player_control_permanent_impulse(local_player_index, &permanent_delta);
}
