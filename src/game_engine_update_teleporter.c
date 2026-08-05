/* game_engine_update_teleporter @0x837477D8 — per-player, per-frame check for standing on a "teleport from"
 * netgame flag: if so, and the found flag differs from the cached one, looks up its paired same-team
 * "teleport to" destination flag; if the destination is physically blocked (a biped/vehicle occupies it),
 * flags/telefrags-counts the blocking player and throttles a "someone is in the way" HUD message (2s
 * cooldown); if clear, plays a teleport sound + screen flash, re-faces the unit along the destination
 * flag's own facing (offset by the unit's incoming approach angle), moves it, and re-primes the cache with
 * the nearest "teleport from" flag at the new position for next frame's invalidation check.
 *
 * DEVIATION: the DB's own decompiler output for this function is corrupted beyond use — its displayed
 * signature has 63 parameters (a2..a63, mostly stack-spilled doubles/floats/ints) versus the DB's real
 * 1-arg prototype `void(int player_index)`, and essentially every `find_netgame_flags`/
 * `collision_get_features_in_sphere` call's arguments are shown shifted/mislabeled as a result. This whole
 * function was reconstructed directly from `disasm_range`, not from the decompiler's variable names. Two
 * confirmations that the disasm-derived register mapping is right: (1) the two `find_netgame_flags` type
 * values recovered (6 then 7) match Halo CE's well-known netgame-flag-type enum values for "Teleporter From"
 * and "Teleporter To"; (2) the recovered `screen_flash_definition` field offsets (built at the stack block
 * later passed to `player_effect_screen_flash`) match that struct's already-established DB layout exactly.
 *
 * FAITHFUL QUIRK: on the "destination flag search failed" path, `console_printf(0, "failed to teleport %d",
 * ...)` prints whatever r7 happens to hold at that point — confirmed via disasm this is the sender flag's
 * own `team_index` (last loaded to set up the second `find_netgame_flags` call's team-filter argument), not
 * a deliberately chosen diagnostic value (player index, flag index, etc.). Reproduced as-is. */

#include <wchar.h>
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/screen_flash_definition.h"
#include "headers/collision_feature_list.h"
#include "headers/collision_plane.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/netgame_flag_type.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"
#include "headers/object_type.h"
#include "headers/multiplayer_game_text_string.h"
#include "headers/screen_flash_priority.h"



/* the "blocked-teleport" screen-flash preset — independent globals in a shared constants block, not a struct */

extern int find_netgame_flags(const real_point3d *location, float distance, float height_delta, int16_t type, int16_t team, int max, int *flags);
extern void biped_get_physics_pill(int biped_index, real_point3d *base, float *height, float *width);
extern uint8_t collision_get_features_in_sphere(unsigned int flags, const real_point3d *center, float radius, float height, float width, int ignore_object_index, collision_feature_list *features);
extern uint8_t collision_features_test_point(const collision_feature_list *features, const real_point3d *point, collision_plane *collision);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern int16_t unit_get_local_player_index(int unit_index);
extern void hud_print_message(int16_t local_player_index, const wchar_t *text);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern void player_effect_screen_flash(int player_index, const screen_flash_definition *screen_flash, float scale);
extern float normalize3d(real_vector3d *v);
extern void object_set_position(int object_index, const real_point3d *position, const real_vector3d *forward, const real_vector3d *up);
extern void player_control_set_facing(int16_t local_player_index, const real_vector3d *forward);
extern void console_printf(uint8_t clear, const char *format, ...);
extern double atan2(double y, double x);
extern double cos(double x);
extern double sin(double x);

void game_engine_update_teleporter(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if (player->unit_index == -1)
        return;

    object_datum *unit_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum;

    /* cached "teleport from" flag index the player was last seen standing on; the multiplayer_player_info
     * teleporter_index field (player_datum+0x70) */
    /* recovered: (int *)(player->multiplayer + 4) -> &player->multiplayer.teleporter_index */
    int *cached_from_flag_index = &player->multiplayer.teleporter_index;

    if (*cached_from_flag_index != -1)
    {
        scenario_netgame_flag *cached_flag =
            (scenario_netgame_flag *)global_scenario->netgame_flags.address + *cached_from_flag_index;

        float dx = unit_object->object.position.n[0] - cached_flag->position.n[0];
        float dy = unit_object->object.position.n[1] - cached_flag->position.n[1];
        float dz = unit_object->object.position.n[2] - cached_flag->position.n[2];
        if (dx * dx + dy * dy + dz * dz > 1.0f)
            *cached_from_flag_index = -1;
    }

    int found_from_flag_index = -1;
    find_netgame_flags(&unit_object->object.position, 0.5f, 0.0f, _netgame_flag_teleporter_source, -1, 1,
                       &found_from_flag_index);
    if (found_from_flag_index == -1 || found_from_flag_index == *cached_from_flag_index)
        return;

    scenario_netgame_flag *sender_flag = (scenario_netgame_flag *)global_scenario->netgame_flags.address
                                        + found_from_flag_index;

    int found_to_flag_index = -1;
    find_netgame_flags(0, 0.0f, 0.0f, _netgame_flag_teleporter_target, sender_flag->team_index, 1, &found_to_flag_index);
    if (found_to_flag_index == -1)
    {
        console_printf(0, "failed to teleport %d", sender_flag->team_index);
        return;
    }

    scenario_netgame_flag *destination_flag = (scenario_netgame_flag *)global_scenario->netgame_flags.address
                                             + found_to_flag_index;

    real_point3d pill_base;
    float        pill_height;
    float        pill_width;
    biped_get_physics_pill(player->unit_index, &pill_base, &pill_height, &pill_width);

    collision_feature_list features;
    uint8_t any_features = collision_get_features_in_sphere(
        (1u << _collision_test_objects_bit) | (1u << _collision_test_objects_bipeds_bit)
      | (1u << _collision_test_objects_vehicles_bit) | (1u << _collision_test_skip_passthrough_bipeds_bit), &destination_flag->position, pill_width * 2.0f + pill_height, pill_height, pill_width, -1,
        &features);

    collision_plane collision;
    if (any_features && collision_features_test_point(&features, &destination_flag->position, &collision))
    {
        /* destination is occupied — flag/telefrag-count the blocker if it's a biped or vehicle */
        if (collision.object_index != -1)
        {
            unit_datum *blocking_object =
                ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, collision.object_index)->datum);

            if ((1 << blocking_object->object.type) & object_mask_unit) /* biped or vehicle */
            {
                int controlling_player_index = blocking_object->unit.player_index;
                if (controlling_player_index != -1)
                {
                    player_datum *blocking_player = DATA_ARRAY_ELEMENT(player_data, player_datum, controlling_player_index);
                    blocking_player->is_blocking_teleporter = 1;
                    blocking_player->telefrag_timeout++;
                }
            }
        }

        if (blocked_message_delay <= 0)
        {
            blocked_message_delay = 120;
            int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
            if (text_tag == -1)
            {
                int16_t local_player_index = unit_get_local_player_index(player->unit_index);
                hud_print_message(local_player_index, L""); /* empty_wide_string — fallback empty message */
            }
            else
            {
                const wchar_t *string = unicode_string_list_get_string(text_tag, _string_teleporter_is_blocked);
                int16_t local_player_index = unit_get_local_player_index(player->unit_index);
                hud_print_message(local_player_index, string);
            }
        }
        else
        {
            blocked_message_delay--;
        }
    }
    else
    {
        /* destination is clear — teleport the unit there */
        if (player->local_player_index != (int16_t)0xFFFF)
            game_engine_play_multiplayer_sound(_multiplayer_sound_teleporter_activate, 0);

        if (player->local_player_index != (int16_t)0xFFFF)
        {
            screen_flash_definition screen_flash;
            memset((char *)&screen_flash + 2, 0, 0x36);
            screen_flash.type = screen_flash_type_1;
            screen_flash.priority = _screen_flash_high_priority;
            screen_flash.duration = duration_1;
            screen_flash.fade_function = (int16_t)fade_function_1;
            screen_flash.max_intensity = max_intensity_1;
            screen_flash.zero_scale_factor = 0.0f;
            screen_flash.screen_flash_color.__s1.alpha = alpha;
            screen_flash.screen_flash_color.__s1.rgb.__s1.red = red_1;
            screen_flash.screen_flash_color.__s1.rgb.__s1.green = green_0;
            screen_flash.screen_flash_color.__s1.rgb.__s1.blue = blue_0;
            player_effect_screen_flash(player_index, &screen_flash, 1.0f);
        }

        /* new facing = destination flag's facing, offset by the unit's approach angle relative to the
         * SENDER flag's facing — i.e. the relative approach angle is preserved across the teleport, replayed
         * against the destination's own orientation. */
        double approach_angle = atan2(unit_object->object.forward.n[1], unit_object->object.forward.n[0]);
        double facing = (double)destination_flag->facing + approach_angle - (double)sender_flag->facing;

        real_vector3d new_forward;
        /* DEVIATION: z is never recomputed here — disasm shows the stack slot backing new_forward.z still
         * holds the unit's OWN forward.z (stashed earlier, before the biped_get_physics_pill/collision calls,
         * and never touched by any of them). x/y are freshly computed from cos/sin; z is carried over as-is. */
        new_forward.n[2] = unit_object->object.forward.n[2];
        new_forward.n[0] = (float)cos(facing);
        new_forward.n[1] = (float)sin(facing);
        normalize3d(&new_forward);

        object_set_position(player->unit_index, &destination_flag->position, &new_forward, 0);

        if (player->local_player_index != (int16_t)0xFFFF)
            player_control_set_facing(player->local_player_index, &new_forward);

        *cached_from_flag_index = -1;
        find_netgame_flags(&unit_object->object.position, 1.0f, 0.0f, _netgame_flag_teleporter_source, -1, 1,
                           cached_from_flag_index);
    }
}
