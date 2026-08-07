/* player_telefrag_effect_start @0x83738698 — kicks off the local-player feedback effects for a
 * telefrag (being killed by a teleporting/spawning player): a white screen flash and a camera shake
 * scaled by `scale`, plus a continuous controller vibration. Only fires for a player with a valid
 * local-player slot.
 *
 * DEVIATION: the DB prototypes for player_effect_update_screen_flash / _camera_shake list a leading
 * __int16 local_player_index and a separate definition pointer, but the call site (per disassembly)
 * passes the player_effect_datum* live-state pointer in r3 and the freshly-built definition in r4,
 * with no third pointer argument — so the effective signature is (effect_state, definition, scale,
 * time_scale). */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/player_effect_globals.h"
#include "headers/screen_flash_definition.h"
#include "headers/camera_shake_definition.h"
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"
#include "headers/render_screen_flash_type.h"
#include "headers/screen_flash_priority.h"


extern void vibrate_player_continuous(int16_t local_player_index, float low_frequency_strength, float high_frequency_strength);
extern void player_effect_update_screen_flash(player_effect_datum *effect_state, const screen_flash_definition *screen_flash, float scale, float time_scale);
extern void player_effect_update_camera_shake(player_effect_datum *effect_state, const camera_shake_definition *camera_shake, float scale, float time_scale);

void player_telefrag_effect_start(int player_index, float scale)
{
    screen_flash_definition screen_flash;
    camera_shake_definition camera_shake;
    memset(&screen_flash, 0, sizeof(screen_flash));
    memset(&camera_shake, 0, sizeof(camera_shake));

    int16_t local_player_index =
        DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index;
    if (local_player_index == -1)
        return;

    screen_flash.zero_scale_factor = 0.0f;
    screen_flash.max_intensity = scale;
    screen_flash.duration = 1.0f;
    camera_shake.duration = 1.0f;
    screen_flash.type = _render_screen_flash_type_lighten;
    screen_flash.priority = _screen_flash_high_priority;
    camera_shake.random_translation_magnitude = scale * 0.01f;
    screen_flash.screen_flash_color.n[0] = global_real_argb_white->n[0];
    screen_flash.screen_flash_color.n[1] = global_real_argb_white->n[1];
    screen_flash.screen_flash_color.n[2] = global_real_argb_white->n[2];
    screen_flash.screen_flash_color.n[3] = global_real_argb_white->n[3];

    player_effect_datum *effect_state = &player_effect_globals->local_player_effect_data[local_player_index];

    vibrate_player_continuous(local_player_index, scale, scale);
    player_effect_update_screen_flash(effect_state, &screen_flash, scale, 1.0f);
    player_effect_update_camera_shake(effect_state, &camera_shake, scale, 1.0f);
}
