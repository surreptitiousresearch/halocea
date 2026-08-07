/* hud_draw_meter @0x8379F770 — draw one HUD meter element (health/shield/energy bar). Resolves the meter's
 * bitmap for the current sequence, computes the min/max alpha endpoints from the passed 0..255 values shaped
 * by the element's alpha_multiplier/bias/minimum, then builds the rasterizer_meter_parameters color set:
 *  - draw_flags&2 (disabled)         -> all colors zeroed
 *  - meter_flags&1 (solid/gradient)  -> either a min<->max gradient interpolated by reference_value
 *                                       (meter_flags&2) or a flat min/max color
 *  - otherwise                       -> a flash color fading over reference_time
 * and finally hands everything to hud_draw_bitmap_with_meter with the fade/opacity-derived tint.
 *
 * DEVIATIONS (disasm-resolved, 0x8379F770-0x8379FB7C):
 *  - reference_time / reference_value are float (f1/f2); the decompiler widened them to double via the
 *    Xbox360 FPR-shadow ABI.
 *  - The tint/fade color is stored to meter_parameters.tint_color ONLY: color32 (arg 8 = r10) is the
 *    literal 0xFFFFFFFF (`li r10,-1` @0x8379FB20). r8/r9 are dead float-shadow GPR slots, and the
 *    decompiler mistook the tint intermediate living in r8 for the argument.
 *  - in_multiplayer (arg 9, stb r1+0x57 @0x8379FB10) is (draw_flags >> 2) & 1, not 0; is_interface_bitmap
 *    (arg 10, stb r1+0x5F @0x8379FB08) is the meter bitmap tag's type == 4. The callee has no param 11.
 *  - float->byte conversions are truncating (fctidz), rendered by the decompiler as __int64/__ROL4__ puns. */

#include <stdint.h>
#include "headers/meter_hud_element_definition.h"
#include "headers/hud_scaling_flags.h"
#include "headers/hud_meter_flags.h"
#include "headers/hud_absolute_placement_definition.h"
#include "headers/hud_placement_definition.h"
#include "headers/rasterizer_meter_parameters.h"
#include "headers/real_rgb_color.h"
#include "headers/real_rectangle2d.h"
#include "headers/bitmap_data.h"
#include "headers/blam_data_globals.h"
#include "headers/hud_draw_flags.h"


#include "headers/global_tag_instances.h"
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern const real_rectangle2d *get_sprite_clip_rect(int bitmap_group_index, int16_t sequence_index, int16_t frame);
extern real_rgb_color *pixel32_to_real_rgb_color(unsigned int pixel, real_rgb_color *color);
extern real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags, const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u);
extern void hud_draw_bitmap_with_meter(rasterizer_meter_parameters *meter_parameters, const bitmap_data *bitmap, const hud_absolute_placement_definition *absolute_placement, const hud_placement_definition *placement, const real_rectangle2d *clip, float scale, float theta, unsigned int color32, uint8_t in_multiplayer, uint8_t is_interface_bitmap);

/* meter->{alpha_multiplier,bias} shape a 0..255 input; clamp to [0,255] then floor at minimum_value. */
static int hud_meter_shape_alpha(const meter_hud_element_definition *meter, uint8_t value)
{
    int product = value * meter->alpha_multiplier + meter->alpha_bias;
    int clamped = product < 0 ? 0 : (product > 255 ? 255 : product);
    return meter->minimum_value <= clamped ? clamped : meter->minimum_value;
}

void hud_draw_meter(int16_t local_player_index, const hud_absolute_placement_definition *placement, const meter_hud_element_definition *meter, uint8_t min_value, uint8_t max_value, int16_t draw_flags, float reference_time, float reference_value)
{
    bitmap_data *bitmap = bitmap_group_get_bitmap_from_sequence(meter->meter_bitmap.index,
            meter->sequence_index, 0);
    if ( !_texture_cache_bitmap_get_hardware_format(bitmap, 0, 1u) )
        return;

    const real_rectangle2d *clip = get_sprite_clip_rect(meter->meter_bitmap.index, meter->sequence_index, 0);

    int min_alpha = hud_meter_shape_alpha(meter, min_value);
    int max_alpha = hud_meter_shape_alpha(meter, max_value);

    float scale = hcex_hud_globals_scale;
    if ( (meter->placement.multiplayer_scaling_flags & (1u << _hud_use_high_resolution_scale_bit)) != 0 )
        scale = (hcex_hud_globals_scale * 0.5f);

    rasterizer_meter_parameters meter_parameters;
    if ( (draw_flags & (1u << _hud_draw_disabled_bit)) != 0 )
    {
        meter_parameters.gradient_min_color = 0;
        meter_parameters.gradient_max_color = 0;
        meter_parameters.flash_color = 0;
    }
    else if ( (meter->meter_flags & (1u << _hud_meter_switch_color_on_state_change)) != 0 )
    {
        if ( (draw_flags & 1) != 0 && (meter->meter_flags & (1u << _hud_meter_interpolates_between_min_max_bit)) != 0 )
        {
            real_rgb_color min_rgb;
            real_rgb_color max_rgb;
            real_rgb_color interpolated;
            pixel32_to_real_rgb_color(meter->min_color, &min_rgb);
            pixel32_to_real_rgb_color(meter->max_color, &max_rgb);

            float u;
            if ( (meter->meter_flags & (1u << _hud_meter_invert_interpolation_value_bit)) != 0 )
                u = (1.0f - reference_value);
            else
                u = reference_value;
            rgb_colors_interpolate(&interpolated, 0, &min_rgb, &max_rgb, u);

            int red = (int)(interpolated.n[0] * 255.0f);
            int green = (int)(interpolated.n[1] * 255.0f);
            int blue = (int)(interpolated.n[2] * 255.0f);
            meter_parameters.flash_color = min_alpha << 24;
            meter_parameters.gradient_max_color =
                    ((red & 0xFF) << 16) | ((green & 0xFF) << 8) | (blue & 0xFF);
            meter_parameters.gradient_min_color =
                    (min_alpha << 24) | ((red & 0xFF) << 16) | ((green & 0xFF) << 8) | (blue & 0xFF);
        }
        else
        {
            unsigned int base_color = (draw_flags & 1) != 0 ? meter->max_color : meter->min_color;
            meter_parameters.flash_color = min_alpha << 24;
            meter_parameters.gradient_min_color = (base_color & 0xFFFFFF) | (min_alpha << 24);
            meter_parameters.gradient_max_color = base_color & 0xFFFFFF;
        }
    }
    else
    {
        float flash_intensity;
        if ( reference_time >= 0.0f && (flash_intensity = (1.0f - reference_time)) >= 0.0f )
        {
            if ( flash_intensity > 1.0f )
                flash_intensity = 1.0f;
        }
        else
        {
            flash_intensity = 0.0f;
        }

        real_rgb_color flash_rgb;
        pixel32_to_real_rgb_color(meter->flash_color, &flash_rgb);
        meter_parameters.gradient_min_color = (min_alpha << 24) | (meter->min_color & 0xFFFFFF);

        int flash_red = (int)((flash_rgb.n[0] * flash_intensity) * 255.0f);
        int flash_green = (int)((flash_rgb.n[1] * flash_intensity) * 255.0f);
        int flash_blue = (int)((flash_rgb.n[2] * flash_intensity) * 255.0f);
        meter_parameters.flash_color = (max_alpha << 24)
                | ((((flash_red & 0xFF) << 16) | ((flash_green & 0xFF) << 8) | (flash_blue & 0xFF)) & 0xFFFFFF);
        meter_parameters.gradient_max_color = meter->max_color & 0xFFFFFF;
    }

    unsigned int empty_color = meter->empty_color;
    meter_parameters.gradient = 1.0f;
    meter_parameters.tint_mode_2 = 1;
    /* invert the empty color's alpha byte: (255 - alpha) << 24 | rgb */
    meter_parameters.background_color =
            (((-1 - (int)((empty_color >> 24) & 0xFF)) & 0xFF) << 24) | (empty_color & 0xFFFFFF);
    meter_parameters.flash_color_is_negative = 0;

    int fade_byte = (int)(meter->fade * 255.0f);
    int inverse_opacity_byte = (int)((1.0f - meter->opacity) * 255.0f);
    unsigned int tint_color = ((fade_byte & 0xFF) << 24) | ((inverse_opacity_byte & 0xFF) << 16)
            | ((inverse_opacity_byte & 0xFF) << 8) | (inverse_opacity_byte & 0xFF);
    /* stored to tint_color and reused as the color32 argument (decompiler folded the field store) */
    meter_parameters.tint_color = tint_color;

    /* meter bitmap tag header word 0 is bitmap_group.type; 4 = the "Interface Bitmap" usage
     * (`lhz r11,0(r29)` / `addi r7,r11,-4` / `cntlzw` / `extrwi r26,r5,1,26` @0x8379F7EC-0x8379F81C) */
    uint16_t *bitmap_group_header = TAG_GET(uint16_t, meter->meter_bitmap.index);
    hud_draw_bitmap_with_meter(&meter_parameters, bitmap, placement, &meter->placement, clip, scale, 0.0f,
            0xFFFFFFFFu, (draw_flags & (1u << _hud_draw_in_multiplayer_bit)) != 0,
            *bitmap_group_header == 4);
}
