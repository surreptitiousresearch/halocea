/* interface_draw_screen 0x836A6868 — apply the active weapon's zoom screen-effect (convolution blur, light
 * enhancement, and desaturation filters) then draw the HUD. The effect parameters come from the weapon HUD
 * interface's zoom screen-effect definition; each filter is gated on being zoomed in (or its "always" flag) and
 * scaled by the script screen-effect value and, optionally, the flashlight power. With no weapon HUD or no
 * effect, the screen effect is cleared. The HUD and post-rasterize step always run for a valid local player.
 *
 * DEVIATION 1: the decompiler leaked the zero-init loop's count (7 qwords) into the stored value
 * (0x700000000LL); the loop actually clears the 56-byte rasterizer_screen_effect_parameters to zero.
 * DEVIATION 2: in the convolution-radius interpolation the decompiler reused the main_get_window_count() return
 * as the scalars_interpolate output pointer and then read the radius as 0; the disassembly writes the
 * interpolated radius to a stack temporary and reads it back, restored here. */

#include <stdint.h>
#include <string.h>
#include "headers/render_globals.h"
#include "headers/hud_screen_effect_mask_only_when_zoomed_flags.h"
#include "headers/hud_screen_effect_convolution_only_when_zoomed_flags.h"
#include "headers/hud_screen_effect_light_enhancement_flags.h"
#include "headers/hud_screen_effect_desaturation_flags.h"
#include "headers/rasterizer_screen_effect_parameters.h"
#include "headers/rasterizer_screen_effect_convolution_type.h"
#include "headers/bitmap_data.h"
#include "headers/bitmap_group.h"
#include "headers/weapon_hud_interface_definition.h"
#include "headers/hud_screen_effect_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


extern int interface_get_weapon_hud_index(float *flashlight_power);
extern int16_t player_control_get_zoom_level(int16_t local_player_index);
extern int16_t main_get_window_count(void);
extern void scalars_interpolate(float a, float b, float t, float *result);
extern float rasterizer_script_screen_effect_get_value(int16_t index);
extern void rasterizer_screen_effect(const rasterizer_screen_effect_parameters *parameters);
extern void hud_draw_screen(void);
extern int game_engine_post_rasterize(void);

void interface_draw_screen(void)
{
    float flashlight_power;
    rasterizer_screen_effect_parameters params;
    int applied_effect;
    int weapon_hud_index;

    if ( (uint16_t)render.local_player_index == 0xFFFF )
        return;

    weapon_hud_index = interface_get_weapon_hud_index(&flashlight_power);
    applied_effect = 0;
    if ( weapon_hud_index != -1 )
    {
        weapon_hud_interface_definition *hud_definition = TAG_GET(weapon_hud_interface_definition, weapon_hud_index);
        if ( hud_definition->screen_effects.count > 0 )
        {
            hud_screen_effect_definition *zoom_def = (hud_screen_effect_definition *)hud_definition->screen_effects.address;
            int16_t zoom_level = player_control_get_zoom_level(render.local_player_index);
            int zoomed = zoom_level != -1;

            memset(&params, 0, sizeof(params));

            /* convolution mask bitmap + which filters reference it */
            if ( zoomed || (zoom_def->mask_flags & (1u << _hud_screen_effect_mask_only_when_zoomed_bit)) == 0 )
            {
                int mask_tag = main_get_window_count() > 1 ? zoom_def->mask_splitscreen.index : zoom_def->mask_fullscreen.index;
                if ( mask_tag != -1 )
                {
                    params.convolution_mask = (bitmap_data *)(TAG_GET(bitmap_group, mask_tag))->bitmaps.address;   /* +100 */
                    params.filter_light_enhancement_uses_convolution_mask = (zoom_def->light_enhancement_flags & (1u << _hud_screen_effect_light_enhancement_uses_convolution_mask_bit)) != 0;
                    params.filter_desaturation_uses_convolution_mask = (zoom_def->desaturation_flags & (1u << _hud_screen_effect_desaturation_uses_convolution_mask_bit)) != 0;
                }
            }

            /* convolution blur radius, interpolated from the field of view (single window only) */
            if ( main_get_window_count() <= 1 && (zoomed || (zoom_def->convolution_flags & (1u << _hud_screen_effect_convolution_only_when_zoomed_bit)) == 0) )
            {
                float convolution_radius;
                if ( zoom_def->convolution_radius_in_bounds[0] == zoom_def->convolution_radius_in_bounds[1] )
                {
                    convolution_radius = zoom_def->convolution_radius_out_bounds[1];
                }
                else
                {
                    float t = ((render.camera.vertical_field_of_view - zoom_def->convolution_radius_in_bounds[0])
                                    / (zoom_def->convolution_radius_in_bounds[1] - zoom_def->convolution_radius_in_bounds[0]));
                    if ( t >= 0.0 )
                    {
                        if ( t > 1.0 )
                            t = 1.0f;
                    }
                    else
                    {
                        t = 0.0f;
                    }
                    scalars_interpolate(zoom_def->convolution_radius_out_bounds[0], zoom_def->convolution_radius_out_bounds[1], t, &convolution_radius);
                }
                if ( convolution_radius > 0.0 )
                {
                    params.convolution_radius = convolution_radius;
                    params.convolution_type = _rasterizer_screen_effect_convolution_type_warp;
                }
            }

            /* light enhancement */
            if ( zoomed || (zoom_def->light_enhancement_flags & (1u << _hud_screen_effect_light_enhancement_only_when_zoomed_bit)) == 0 )
            {
                float intensity = zoom_def->light_enhancement_intensity;
                if ( (zoom_def->light_enhancement_flags & (1u << _hud_screen_effect_light_enhancement_connect_to_flashlight_bit)) != 0 )
                {
                    float scale;
                    if ( flashlight_power >= 0.0 )
                        scale = flashlight_power <= 1.0 ? flashlight_power : 1.0f;
                    else
                        scale = 0.0f;
                    intensity = (scale * zoom_def->light_enhancement_intensity);
                }
                float script_value = rasterizer_script_screen_effect_get_value(zoom_def->light_enhancement_script_source);
                if ( script_value >= 0.0 )
                {
                    if ( script_value > 1.0 )
                        script_value = 1.0f;
                }
                else
                {
                    script_value = 0.0f;
                }
                if ( (script_value * intensity) > 0.0 )
                    params.filter_light_enhancement_intensity = (script_value * intensity);
            }

            /* desaturation */
            if ( zoomed || (zoom_def->desaturation_flags & (1u << _hud_screen_effect_desaturation_only_when_zoomed_bit)) == 0 )
            {
                float intensity = zoom_def->desaturation_intensity;
                if ( (zoom_def->desaturation_flags & (1u << _hud_screen_effect_desaturation_connect_to_flashlight_bit)) != 0 )
                {
                    float scale;
                    if ( flashlight_power >= 0.0 )
                        scale = flashlight_power <= 1.0 ? flashlight_power : 1.0f;
                    else
                        scale = 0.0f;
                    intensity = (scale * zoom_def->desaturation_intensity);
                }
                float script_value = rasterizer_script_screen_effect_get_value(zoom_def->desaturation_script_source);
                if ( script_value >= 0.0 )
                {
                    if ( script_value > 1.0 )
                        script_value = 1.0f;
                }
                else
                {
                    script_value = 0.0f;
                }
                if ( (script_value * intensity) > 0.0 )
                {
                    params.filter_desaturation_intensity = (script_value * intensity);
                    params.filter_desaturation_is_additive = (zoom_def->desaturation_flags & (1u << _hud_screen_effect_desaturation_is_additive_bit)) != 0;
                    params.filter_desaturation_tint.n[1] = zoom_def->desaturation_tint.n[1];
                    params.filter_desaturation_tint.n[0] = zoom_def->desaturation_tint.n[0];
                    params.filter_desaturation_tint.n[2] = zoom_def->desaturation_tint.n[2];
                }
            }

            rasterizer_screen_effect(&params);
            applied_effect = 1;
        }
    }

    if ( !applied_effect )
        rasterizer_screen_effect(0);

    hud_draw_screen();  /* called for side effects; return unused (game_engine_post_rasterize is 0-arg) */
    game_engine_post_rasterize();
}
