/* player_ui_set_active_player_profile @0x83699888 — install a player profile as the active profile for a local
 * player: record the source profile index, copy the 1988-byte profile into the player's UI slot, push the
 * control bindings into the input abstraction layer, build rasterizer settings from the profile's video
 * preferences, force model/effects quality and lens/detail draw flags on, and (for a real, non-default
 * profile) remember player 1's profile.
 *
 * The source profile index is stored in _player_data.profile_index (@1980). */

#include <stdint.h>
#include <string.h>
#include "headers/player_ui_globals.h"
#include "headers/rasterizer_settings_parameters.h"

extern void set_local_player_controls_from_player_profile(int16_t local_player_index);
extern void video_settings_construct_rasterizer_parameters(player_profile *profile, rasterizer_settings_parameters *parameters);
extern void player_ui_remember_player1_profile(uint8_t write_to_disk);

extern struct {
    int rasterizer_model_quality_level;
    int rasterizer_effects_level;
    unsigned char draw_lens_flares;
    unsigned char draw_detail_objects;
} rasterizer_debug_options;

void player_ui_set_active_player_profile(int16_t local_player_index, int profile_index, player_profile *profile)
{
    player_profile *slot = &player_ui_globals.player_data[local_player_index].profile;

    player_ui_globals.player_data[local_player_index].profile_index = profile_index;
    memcpy(&player_ui_globals.player_data[local_player_index], profile, sizeof(*profile));
    set_local_player_controls_from_player_profile(local_player_index);

    rasterizer_settings_parameters parameters;
    rasterizer_debug_options.rasterizer_model_quality_level = 2;
    video_settings_construct_rasterizer_parameters(slot, &parameters);
    rasterizer_debug_options.rasterizer_effects_level = 2;
    rasterizer_debug_options.draw_lens_flares = 1;
    rasterizer_debug_options.draw_detail_objects = 1;

    if ( profile_index != -1 )
        player_ui_remember_player1_profile(1u);
}
