/* player_effect_start_client @0x837387B8 — apply the client-side feedback for a damage hit to a local player:
 * screen flash, camera impulse, camera shake, controller vibration and an unspatialized impulse sound, all
 * driven by the damage effect definition's sub-definitions. When the hit did real damage and has a known
 * attacker, the directional damage indicator is also lit: the attacker's position relative to the player's
 * head is projected into the camera's (left, forward, up) frame and the nearest of the four indicator
 * quadrants is flagged. The global random seed is held across the whole routine.
 *
 * DEVIATION 1: the DB prototypes for player_effect_update_screen_flash and player_effect_update_camera_shake
 * carry a spurious leading local_player_index; the actual ABI (and the already-reconstructed callees) take
 * (effect_state, definition, scale, time_scale). The decompiler's FPR-shadow misread (a phantom 0x82000000
 * argument and a shifted first parameter) is corrected here against the disassembly.
 * DEVIATION 2: scale/total_damage are floats (rendered as doubles by the FPR-shadow ABI); atan2's two operands
 * were packed into one long double, restored to atan2(forward_component, left_component). The camera-relative
 * projection (decompiler cross-product soup over camera floats 8..13) is left = forward x up plus three dot
 * products. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/player_effect_globals.h"
#include "headers/damage_data.h"
#include "headers/damage_effect_definition.h"
#include "headers/player_datum.h"
#include "headers/observer_result.h"
#include "headers/screen_flash_definition.h"
#include "headers/camera_shake_definition.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/damage_definition_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

typedef struct camera_impulse_definition camera_impulse_definition;
typedef struct vibrate_definition vibrate_definition;


extern void lock_global_random_seed(void);
extern void unlock_global_random_seed(void);
extern void player_effect_update_screen_flash(player_effect_datum *effect_state, const screen_flash_definition *screen_flash, float scale, float time_scale);
extern void player_effect_update_camera_impulse(int16_t local_player_index, player_effect_datum *datum, const camera_impulse_definition *camera_impulse, const real_vector3d *direction, float scale, float time_scale);
extern void player_effect_update_camera_shake(player_effect_datum *effect_state, const camera_shake_definition *camera_shake, float scale, float time_scale);
extern void vibrate_player_impulse(int16_t local_player_index, const vibrate_definition *definition, float scale, float time_scale);
extern int unspatialized_impulse_sound_new(int definition_index, float scale);
extern int local_player_get_player_index(int16_t local_player_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern const observer_result *observer_get_camera(int16_t local_player_index);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern float normalize3d(real_vector3d *v);
extern double atan2(double y, double x);
extern double __fabs(double x);

void player_effect_start_client(int player_index, const damage_data *damage, const real_vector3d *direction,
                                float scale, float total_damage)
{
    int16_t local_player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index;
    lock_global_random_seed(); /* attested void(void): decompiler threaded a phantom seed handle */

    if (local_player != -1)
    {
        player_effect_datum *effect_datum = &player_effect_globals->local_player_effect_data[local_player];
        damage_effect_definition *definition = TAG_GET(damage_effect_definition, damage->definition_index);

        player_effect_update_screen_flash(effect_datum, &definition->screen_flash,
                                          scale, 1.0f);
        player_effect_update_camera_impulse(local_player, effect_datum,
                                            &definition->camera_impulse, direction,
                                            scale, 1.0f);
        player_effect_update_camera_shake(effect_datum, &definition->camera_shake,
                                          scale, 1.0f);
        vibrate_player_impulse(local_player, &definition->vibrate, scale, 1.0f);

        int sound_definition = definition->sound.index;
        if (sound_definition != -1)
            unspatialized_impulse_sound_new(sound_definition, 1.0f);

        if (total_damage > 0.0f && damage->owner_object_index != -1)
        {
            unsigned char *damage_indicator_ticks = effect_datum->damage_indicator_ticks;
            if ((definition->damage.flags & (1u << _damage_draw_indicators_down_bit)) != 0) /* "non-directional" flag */
            {
                damage_indicator_ticks[2] = 1;
            }
            else
            {
                int local_unit;
                if (local_player_get_player_index(local_player) == -1)
                    local_unit = -1;
                else
                    local_unit = DATA_ARRAY_ELEMENT(player_data, player_datum,
                                     local_player_get_player_index(local_player))->unit_index;

                void *unit = object_try_and_get_and_verify_type(local_unit, object_mask_unit);
                void *attacker = object_try_and_get_and_verify_type(damage->owner_object_index, object_mask_all);
                if (unit && attacker)
                {
                    const observer_result *camera = observer_get_camera(local_player);
                    if (camera)
                    {
                        real_point3d head_position;
                        real_point3d attacker_position;
                        unit_get_head_position(local_unit, &head_position);
                        object_get_origin(damage->owner_object_index, &attacker_position);

                        real_vector3d delta;
                        delta.n[0] = attacker_position.n[0] - head_position.n[0];
                        delta.n[1] = attacker_position.n[1] - head_position.n[1];
                        delta.n[2] = attacker_position.n[2] - head_position.n[2];

                        /* camera left axis = forward x up */
                        real_vector3d left;
                        left.n[0] = camera->forward.n[1] * camera->up.n[2] - camera->forward.n[2] * camera->up.n[1];
                        left.n[1] = camera->forward.n[2] * camera->up.n[0] - camera->forward.n[0] * camera->up.n[2];
                        left.n[2] = camera->forward.n[0] * camera->up.n[1] - camera->forward.n[1] * camera->up.n[0];

                        real_vector3d relative; /* attacker direction in camera (left, forward, up) frame */
                        relative.n[0] = left.n[0] * delta.n[0] + left.n[1] * delta.n[1] + left.n[2] * delta.n[2];
                        relative.n[1] = camera->forward.n[0] * delta.n[0] + camera->forward.n[1] * delta.n[1]
                                      + camera->forward.n[2] * delta.n[2];
                        relative.n[2] = camera->up.n[0] * delta.n[0] + camera->up.n[1] * delta.n[1]
                                      + camera->up.n[2] * delta.n[2];

                        if (normalize3d(&relative) != 0.0f)
                        {
                            if (__fabs(relative.n[2]) > 0.5f)
                            {
                                if (relative.n[2] <= 0.0f)
                                    damage_indicator_ticks[2] = 1;
                                else
                                    damage_indicator_ticks[0] = 1;
                            }

                            float angle = (float)atan2(relative.n[1], relative.n[0]);
                            if (angle < 0.78539819f || angle > 2.3561945f)
                            {
                                if (__fabs(angle) <= 1.5707964f)
                                    damage_indicator_ticks[3] = 1;
                                else
                                    damage_indicator_ticks[1] = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    unlock_global_random_seed();
}
