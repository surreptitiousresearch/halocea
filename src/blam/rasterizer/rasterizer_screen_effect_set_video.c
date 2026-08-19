/* rasterizer_screen_effect_set_video @0x83724508 — turns on the cinematic "video" screen effect
 * (scanlines + noise). Resets the live parameter block and binds the scanline / noise bitmaps from
 * the game-globals rasterizer data. No-op if the effect globals aren't allocated or either map tag
 * reference is null. */

#include <stdint.h>
#include <string.h>
#include "headers/cinematic_screen_effect_globals.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/bitmap_data.h"
#include "headers/bitmap_group.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


void rasterizer_screen_effect_set_video(int16_t video_overbright_mode, float video_noise_intensity)
{
    struct cinematic_screen_effect_globals *effect = cinematic_screen_effect_globals;

    if (effect
        && global_rasterizer_data->screen_effect_video_scanline_map.index != -1
        && global_rasterizer_data->screen_effect_video_noise_map.index != -1)
    {
        memset(effect, 0, 0x38u);
        effect->parameters.video_overbright_mode = video_overbright_mode;
        effect->parameters.video_on = 1;
        effect->convolution_radius[0] = 0.0f;
        effect->convolution_radius[1] = 0.0f;
        effect->convolution_time[0] = 0.0f;
        effect->convolution_time[1] = 0.0f;
        effect->filter_light_enhancement_intensity[0] = 0.0f;
        effect->filter_light_enhancement_intensity[1] = 0.0f;
        effect->filter_desaturation_intensity[0] = 0.0f;
        effect->filter_desaturation_intensity[1] = 0.0f;
        effect->filter_time[0] = 0.0f;
        effect->filter_time[1] = 0.0f;

        /* bitmap_group.bitmaps (+0x60) is a tag_block; its .address (+100) is the first bitmap_data. */
        effect->parameters.video_scanline_map = (bitmap_data *)(TAG_GET(bitmap_group, global_rasterizer_data->screen_effect_video_scanline_map.index))->bitmaps.address;
        effect->parameters.video_noise_intensity = video_noise_intensity;
        effect->parameters.video_noise_map_scale = 1.0f;
        effect->parameters.video_noise_map = (bitmap_data *)(TAG_GET(bitmap_group, global_rasterizer_data->screen_effect_video_noise_map.index))->bitmaps.address;
    }
}
