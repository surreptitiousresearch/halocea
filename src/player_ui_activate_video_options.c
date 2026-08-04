/* player_ui_activate_video_options @0x836989E0 — apply the "high" video option preset for a player
 * profile: force model quality and effects to level 2 and enable lens flares and detail objects.
 * The rasterizer_settings_parameters block is (re)built from the profile as a side effect. Returns 0. */

#include "headers/player_profile.h"
#include "headers/rasterizer_debug_options.h"
#include "headers/rasterizer_settings_parameters.h"

extern void video_settings_construct_rasterizer_parameters(player_profile *profile, rasterizer_settings_parameters *parameters);

int player_ui_activate_video_options(player_profile *profile)
{
    rasterizer_settings_parameters parameters;

    rasterizer_debug_options.rasterizer_model_quality_level = 2;
    video_settings_construct_rasterizer_parameters(profile, &parameters);
    rasterizer_debug_options.rasterizer_effects_level = 2;
    rasterizer_debug_options.draw_lens_flares = 1;
    rasterizer_debug_options.draw_detail_objects = 1;
    return 0;
}
