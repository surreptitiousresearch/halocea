/* player_effect_get_camera_effect_matrix @0x83738AB0 — build the combined camera-shake/impulse matrix
 * for a local player's viewport this frame. Two independent systems are composed:
 *  (1) an "impulse" (jitter + direction-scaled translation, decaying via a transition function) that
 *      becomes the base matrix (or identity, if no impulse is active);
 *  (2) a "shake" (periodic rotation/translation with a random component, falling off over its
 *      duration) that is generated into a temporary matrix and multiplied onto the base.
 * A separate scripted (script-driven) camera effect, gated by `player_effect_globals->global_flags & 1`,
 * fully overrides both systems and returns its own randomized rotation/translation matrix instead.
 *
 * DEVIATION: the decompiler shows the shake matrix's output argument as two different phantom
 * variables (`v49`, then reassigned from a bogus `(real_matrix4x3 *)game_time_get_elapsed()` cast into
 * `v54`) for the two `get_shake_matrix` calls. Confirmed via disasm both calls actually target the SAME
 * local `shake_matrix` (r5 in both cases — `get_shake_matrix`'s pointer arg lands in r5, not r3, since
 * its two float args each consume a dead GPR shadow slot first). The first call's result is entirely
 * overwritten by the second before anything reads it — a genuine dead computation in the original
 * binary, reproduced as-is rather than removed.
 *
 * FAITHFUL QUIRK: in the scripted-effect branch, the three `real_seed_random_range` results pair with
 * the rotation/translation axes in reverse call order (the 1st call's result multiplies axis n[2], the
 * 3rd call's result multiplies axis n[0]) — confirmed via disasm register tracing to be exactly what the
 * decompiler shows, not a mis-transcription; kept verbatim. */

#include <stdint.h>
#include "headers/player_effect_globals.h"
#include "headers/real_matrix4x3.h"
#include "headers/screen_flash_just_started_flags.h"
#include "headers/scripted_effect_flags.h"
#include "headers/blam_data_globals.h"


extern void *memcpy(void *dest, const void *src, unsigned int size);
extern int game_time_get(void);
extern int16_t game_time_get_elapsed(void);
extern double cos(double x);
extern double sin(double x);
extern void vibrate_player_set_scale(float scale);
extern void vibrate_player_continuous(int16_t local_player_index, float low_frequency_strength, float high_frequency_strength);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern void matrix4x3_rotation_from_angles(real_matrix4x3 *matrix, float yaw, float pitch, float roll);
extern void matrix4x3_rotation_from_axis_and_angle(real_matrix4x3 *matrix, const real_vector3d *axis, float sine, float cosine);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);
extern float transition_function_evaluate(int16_t function_type, float value);
extern float periodic_function_evaluate(int16_t function_type, float time);
extern void get_shake_matrix(float translation_magnitude, float rotation_magnitude, real_matrix4x3 *shake_matrix);

void player_effect_get_camera_effect_matrix(int16_t local_player_index, real_matrix4x3 *matrix)
{
    if (local_player_index == -1)
        return;

    game_time_get();

    if (player_effect_globals->global_flags & (1u << _scripted_effect_running_bit))
    {
        scripted_player_effect_definition *scripted_effect = &player_effect_globals->scripted_effect;
        float max_intensity = scripted_effect->max_intensity;
        memcpy(matrix, global_identity4x3, sizeof(real_matrix4x3));

        if (scripted_effect->timer <= 0)
        {
            if (player_effect_globals->global_flags & (1u << _scripted_effect_ending_bit))
            {
                player_effect_globals->global_flags &= ~(1u << _scripted_effect_running_bit);
                vibrate_player_set_scale(0.0f);
            }
        }
        else
        {
            if (player_effect_globals->global_flags & (1u << _scripted_effect_ending_bit))
                max_intensity = ((float)scripted_effect->timer / (float)scripted_effect->total_time) * max_intensity;
            else
                max_intensity = (1.0f - (float)scripted_effect->timer / (float)scripted_effect->total_time) * max_intensity;
            scripted_effect->timer -= game_time_get_elapsed();
        }

        if (player_effect_globals->global_flags & (1u << _scripted_effect_running_bit))
        {
            if (max_intensity >= 0.0f)
            {
                if (max_intensity > 1.0f)
                    max_intensity = 1.0f;
            }
            else
            {
                max_intensity = 0.0f;
            }

            vibrate_player_set_scale(max_intensity);

            float random_roll = real_seed_random_range(get_global_local_random_seed_address(), -1.0f, 1.0f);
            float random_pitch = real_seed_random_range(get_global_local_random_seed_address(), -1.0f, 1.0f);
            float random_yaw = real_seed_random_range(get_global_local_random_seed_address(), -1.0f, 1.0f);
            matrix4x3_rotation_from_angles(matrix,
                scripted_effect->max_rotation.n[0] * random_yaw * max_intensity,
                scripted_effect->max_rotation.n[1] * random_pitch * max_intensity,
                scripted_effect->max_rotation.n[2] * random_roll * max_intensity);

            float random_tz = real_seed_random_range(get_global_local_random_seed_address(), -1.0f, 1.0f);
            float random_tx = real_seed_random_range(get_global_local_random_seed_address(), -1.0f, 1.0f);
            float random_ty = real_seed_random_range(get_global_local_random_seed_address(), -1.0f, 1.0f);
            matrix->n[3][2] = (scripted_effect->max_translation.n[2] * random_tz) * max_intensity;
            matrix->n[3][1] = (scripted_effect->max_translation.n[0] * random_tx) * max_intensity;
            matrix->n[3][0] = (scripted_effect->max_translation.n[1] * random_ty) * max_intensity;
        }
        return;
    }

    player_effect_datum *effect = &player_effect_globals->local_player_effect_data[local_player_index];
    real_matrix4x3 impulse_matrix;
    const real_matrix4x3 *impulse_source;

    if (effect->camera_impulse_time_left > 0 || (effect->flags & (1u << _camera_impulse_just_started_bit)))
    {
        float impulse_scale;
        if (effect->flags & (1u << _camera_impulse_just_started_bit))
        {
            impulse_scale = 1.0f;
        }
        else
        {
            impulse_scale = transition_function_evaluate(effect->camera_impulse.temporary_transition,
                1.0f - (effect->camera_impulse.temporary_duration - (float)effect->camera_impulse_time_left)
                        / effect->camera_impulse.temporary_duration)
                * effect->camera_impulse.temporary_zero_scale_factor;
        }

        float dir_x = effect->direction.n[0];
        float dir_z = effect->direction.n[2];
        float dir_y = effect->direction.n[1];
        effect->flags &= ~(1u << _camera_impulse_just_started_bit);

        real_vector3d axis;
        axis.n[0] = dir_z * global_up3d->n[1] - dir_y * global_up3d->n[2];
        axis.n[1] = dir_x * global_up3d->n[2] - global_up3d->n[0] * dir_z;
        axis.n[2] = global_up3d->n[0] * dir_y - dir_x * global_up3d->n[1];

        float rotation_angle = effect->camera_impulse.temporary_rotation * impulse_scale;
        float cosine = (float)cos(rotation_angle);
        float sine = (float)sin(rotation_angle);
        matrix4x3_rotation_from_axis_and_angle(&impulse_matrix, &axis, sine, cosine);

        float translation = effect->camera_impulse.temporary_translation * impulse_scale;
        impulse_matrix.n[3][0] = effect->jitter.n[0] * impulse_scale + effect->direction.n[0] * translation;
        impulse_matrix.n[3][1] = effect->jitter.n[1] * impulse_scale + effect->direction.n[1] * translation;
        impulse_matrix.n[3][2] = effect->jitter.n[2] * impulse_scale + effect->direction.n[2] * translation;

        int16_t elapsed = game_time_get_elapsed();
        impulse_source = &impulse_matrix;
        effect->camera_impulse_time_left -= elapsed;
    }
    else
    {
        impulse_source = global_identity4x3;
    }

    memcpy(matrix, impulse_source, sizeof(real_matrix4x3));

    int16_t camera_shake_time_left = effect->camera_shake_time_left;
    if (camera_shake_time_left > 0 || (effect->flags & (1u << _camera_shake_just_started_bit)))
    {
        real_matrix4x3 shake_matrix;
        memcpy(&shake_matrix, global_identity4x3, sizeof(shake_matrix));

        float shake_scale;
        if (effect->flags & (1u << _camera_shake_just_started_bit))
        {
            shake_scale = 1.0f;
        }
        else
        {
            shake_scale = transition_function_evaluate(effect->camera_shake.falloff_transition_function,
                1.0f - (effect->camera_shake.duration - (float)(unsigned int)camera_shake_time_left)
                        / effect->camera_shake.duration)
                * effect->camera_shake.zero_scale_factor;
        }

        float periodic_phase = (effect->camera_shake.duration - (float)camera_shake_time_left) / effect->camera_shake.periodic_period;
        float periodic_value = periodic_function_evaluate(effect->camera_shake.periodic_function, periodic_phase);
        float weighted = (effect->camera_shake.periodic_weight * periodic_value + (1.0f - effect->camera_shake.periodic_weight)) * shake_scale;

        float random_translation = effect->camera_shake.random_translation_magnitude * weighted;
        if (random_translation <= 0.0f)
            random_translation = 0.0f;

        float random_rotation = effect->camera_shake.random_rotation_magnitude * weighted;
        if (random_rotation <= 0.0f)
            random_rotation = 0.0f;

        effect->flags &= ~(1u << _camera_shake_just_started_bit);

        /* DEVIATION: this first get_shake_matrix result is immediately overwritten below before
         * anything reads it — a dead computation confirmed via disasm, reproduced faithfully. */
        get_shake_matrix(effect->continuous_effect.translational_shake + random_translation,
                         effect->continuous_effect.rotational_shake + random_rotation, &shake_matrix);

        vibrate_player_continuous(local_player_index, effect->continuous_effect.vibrate_frequencies[0],
                                  effect->continuous_effect.vibrate_frequencies[1]);

        int16_t new_timer = (int16_t)(game_time_get_elapsed() + effect->continuous_effect_timer);
        effect->continuous_effect_timer = new_timer;
        if (new_timer > 0)
        {
            effect->continuous_effect_timer = 0;
            effect->continuous_effect.vibrate_frequencies[0] = 0.0f;
            effect->continuous_effect.vibrate_frequencies[1] = 0.0f;
            effect->continuous_effect.translational_shake = 0.0f;
            effect->continuous_effect.rotational_shake = 0.0f;
        }

        get_shake_matrix(random_translation, random_rotation, &shake_matrix);
        effect->camera_shake_time_left -= game_time_get_elapsed();
        matrix4x3_multiply(matrix, &shake_matrix, matrix);
    }
}
