/* hud_draw_numbers @0x8379FB90 — render a numeric HUD element (score/ammo/timer digit group) from the
 * shared "counter" interface tag (interface index 11) into the number bitmap group it references. Digits
 * are drawn right-to-left from a pen position that hud_calculate_point anchors to the element's corner,
 * then shifted left for right/centre alignment: [minus][integer digits].[fractional digits][x1000 suffix].
 * The pen advance per glyph is a signed font-width byte in the interface tag (+0x11); the decimal point
 * uses a second metric byte (+0x14). number_flags & 4 enables the "over 1000" abbreviation (value/1000
 * with a K/x1000 suffix glyph, frame 13/14); number_flags & 1 forces leading zeros. draw_flags selects
 * the color: &2 = disabled color, &1 = flashing color (get_flash_color), else the base color; &4 marks
 * the multiplayer (in-game) placement mode for hud_calculate_point.
 *
 * Reconstructed by per-instruction disasm decode (Hex-Rays "local variable allocation has failed"; the
 * float override_scale burns the r7 GPR shadow and every fcfid/fctiwz digit round-trip decompiled to
 * __int64/HIDWORD=0x82000000 puns). DEVIATIONS, all disasm-verified (0x8379FB90-0x837A0300):
 * - r30 = *(unsigned char **)TAG_INSTANCE(interface tag 11) is the interface_definition; interface bytes
 *   +0x11 (signed) = glyph advance width, +0x14 (signed) = decimal-point spacing metric, +0xC (int) =
 *   the number bitmap group tag index (matches hud_render_timer's read of +0x11).
 * - hud_draw_bitmap_direct's float scale/theta args (5,6) burn the r7/r8 shadows, so its color and
 *   is_interface_bitmap really travel in r9/r10; is_interface_bitmap = (bitmap group definition's first
 *   u16 == 4) (the group->type==4 test recorded for crosshairs_draw).
 * - every pen position/advance is (int)(float expr) truncated then narrowed to int16 (fctiwz + lhz), and
 *   the advances are the single-rounding fnmsubs -(advance*scale - pen_x) = pen_x - advance*scale.
 * - the corner switch (bdzf jump table on absolute_placement->corner) resolves to: 1/3 = pen at origin.__s1.x;
 *   0/2 = origin.__s1.x + ((total-2)*advance + decimal_metric)*scale; default(>=4) = origin.__s1.x +
 *   ((total-1)*advance + decimal_metric)*scale*0.5.
 * - faithful quirk: number_flags & 4 with an in-range value overwrites decimal_value with 10*value BEFORE
 *   value is divided by 1000, so the fractional field shows the original low digits. */

#include <stdint.h>
#include "headers/number_hud_element_definition.h"
#include "headers/hud_scaling_flags.h"
#include "headers/hud_number_show_flags.h"
#include "headers/hud_absolute_placement_definition.h"
#include "headers/bitmap_data.h"
#include "headers/point2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/global_tag_instances.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_type.h"
#include "headers/hud_corner.h"
#include "headers/interface_tag_index.h"
#include "headers/hud_number_definition.h"
#include "headers/blam_data_globals.h"
#include "headers/hud_draw_flags.h"


#include "headers/hud_placement_definition.h"
extern int interface_get_tag_index(int16_t interface_tag_index);
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern int16_t abs16(int16_t value);
extern unsigned int get_flash_color(const hud_color_definition *hud_color_def, int reference_value);
extern void hud_calculate_point(int16_t local_player_index, const hud_absolute_placement_definition *absolute_placement, const hud_placement_definition *placement, const bitmap_data *bitmap, uint8_t in_multiplayer, float override_scale, point2d *result);
extern void hud_retrieve_bitmap_and_bounding_rect(int bitmap_group_index, int16_t sequence_index, int16_t frame_index, const bitmap_data **bitmap, const real_rectangle2d **clip);
extern void hud_draw_bitmap_direct(const bitmap_data *bitmap, int16_t placement, const point2d *point, const real_rectangle2d *clip, float scale, float theta, unsigned int color, uint8_t is_interface_bitmap);

static void hud_draw_number_glyph(int bitmap_group_index, int16_t frame, int16_t corner,
        int16_t x, int16_t y, float scale, unsigned int color, uint8_t is_interface_bitmap)
{
    const bitmap_data *glyph = 0;
    const real_rectangle2d *clip = 0;
    hud_retrieve_bitmap_and_bounding_rect(bitmap_group_index, 0, frame, &glyph, &clip);

    point2d point;
    point.__s1.x = x;
    point.__s1.y = y;
    hud_draw_bitmap_direct(glyph, corner, &point, clip, scale, 0.0f, color, is_interface_bitmap);
}

void hud_draw_numbers(int16_t local_player_index, const hud_absolute_placement_definition *placement, const number_hud_element_definition *numbers, int16_t value, int16_t decimal_value, int16_t draw_flags, int flash_reference_time, float override_scale)
{
    int tag_index = interface_get_tag_index(_interface_hud_digits);
    if ( tag_index == -1 )
        return;

    const hud_number_definition *interface_definition =
        TAG_GET(hud_number_definition, tag_index);
    int bitmap_group_index = interface_definition->number_bitmap.index;
    bitmap_group *number_bitmap_group = TAG_GET(bitmap_group, bitmap_group_index);

    bitmap_data *bitmap = bitmap_group_get_bitmap_from_sequence(bitmap_group_index, 0, 0);
    uint8_t value_in_range = ((value >= 0) + ((unsigned int)value <= 0x3E7)) & 1;
    if ( !_texture_cache_bitmap_get_hardware_format(bitmap, 0, 1) )
        return;

    uint8_t value_is_negative = (unsigned int)value >> 31;

    /* total field count = integer digits + (decimal point + fractional digits) */
    char fractional_digits = numbers->fractional_digits;
    int decimal_field_count;
    if ( !fractional_digits || decimal_value == -1 )
        decimal_field_count = 0;
    else
        decimal_field_count = (fractional_digits <= 4 ? fractional_digits : 4) + 1;
    float total_field_count = (float)(numbers->digits + decimal_field_count);

    float decimal_metric = fractional_digits ? (float)(signed char)interface_definition->decimal_point_width : 0.0f;
    float advance = (float)(signed char)interface_definition->screen_width;

    float scale = override_scale;
    if ( scale <= 0.0f )
        scale = hcex_hud_globals_scale;
    if ( numbers->placement.multiplayer_scaling_flags & (1u << _hud_use_high_resolution_scale_bit) )
        scale = scale * 0.5f;

    if ( numbers->number_flags & (1u << _hud_number_show_trailing_m) )
    {
        total_field_count = total_field_count + 1.0f;
        if ( value_in_range )
        {
            int16_t original_value = value;
            value /= 1000;
            decimal_value = 10 * original_value;
        }
    }

    int16_t abs_value = abs16(value);

    point2d origin;
    hud_calculate_point(local_player_index, placement, &numbers->placement, 0,
            (draw_flags & (1u << _hud_draw_in_multiplayer_bit)) != 0, 0.0f, &origin);

    /* corner alignment: set the right-hand pen x for right-to-left digit layout */
    int16_t pen_x;
    int16_t corner = placement->corner;
    if ( corner == _hud_corner_top_right || corner == _hud_corner_bottom_right )
    {
        pen_x = origin.__s1.x;
    }
    else if ( corner == _hud_corner_top_left || corner == _hud_corner_bottom_left )
    {
        pen_x = (int16_t)(int)((((total_field_count - 2.0f) * advance) + decimal_metric) * scale
                              + (float)origin.__s1.x);
    }
    else
    {
        pen_x = (int16_t)(int)(((((total_field_count - 1.0f) * advance) + decimal_metric) * scale) * 0.5f
                              + (float)origin.__s1.x);
    }

    if ( !bitmap )
        return;

    unsigned int color;
    if ( draw_flags & (1u << _hud_draw_disabled_bit) )
        color = numbers->colors.disabled_color;
    else if ( draw_flags & (1u << _hud_draw_flashing_bit) )
        color = get_flash_color(&numbers->colors, flash_reference_time);
    else
        color = numbers->colors.color;

    int16_t pen_y = origin.__s1.y;
    uint8_t is_interface_bitmap = number_bitmap_group->type == _bitmap_group_type_interface_bitmaps;

    /* "over 1000" suffix glyph (frame 14 in range, 13 out of range) */
    if ( numbers->number_flags & (1u << _hud_number_show_trailing_m) )
    {
        hud_draw_number_glyph(bitmap_group_index, value_in_range ? 14 : 13, placement->corner,
                pen_x, pen_y, scale, color, is_interface_bitmap);
        pen_x = (int16_t)(int)-((advance * scale) - (float)pen_x);
    }

    /* fractional part + decimal point */
    if ( fractional_digits && decimal_value >= 0 )
    {
        int fractional_count = fractional_digits > 4 ? 4 : fractional_digits;
        for ( int k = fractional_count; k < 4; ++k )
            decimal_value /= 10;

        for ( int j = 0; j < fractional_count; ++j )
        {
            hud_draw_number_glyph(bitmap_group_index, decimal_value % 10, placement->corner,
                    pen_x, pen_y, scale, color, is_interface_bitmap);
            pen_x = (int16_t)(int)-((advance * scale) - (float)pen_x);
            decimal_value /= 10;
        }

        /* decimal point (frame 10): nudged right one advance then left by the decimal metric */
        int16_t decimal_anchor = (int16_t)(int)((advance * scale) + (float)pen_x);
        int16_t decimal_x = (int16_t)(int)-((decimal_metric * scale) - (float)decimal_anchor);
        hud_draw_number_glyph(bitmap_group_index, 10, placement->corner,
                decimal_x, pen_y, scale, color, is_interface_bitmap);
        pen_x = (int16_t)(int)-((advance * scale) - (float)decimal_x);
    }

    /* integer digits (least significant first, right-to-left) */
    char digits = numbers->digits;
    if ( digits > 0 )
    {
        for ( int i = 0; i < digits; ++i )
        {
            if ( !abs_value && (numbers->number_flags & (1u << _hud_number_show_all_leading_zeros_bit)) == 0 )
                break;
            hud_draw_number_glyph(bitmap_group_index, abs_value % 10, placement->corner,
                    pen_x, pen_y, scale, color, is_interface_bitmap);
            pen_x = (int16_t)(int)-((advance * scale) - (float)pen_x);
            abs_value /= 10;
        }
    }

    /* minus sign (frame 12) */
    if ( value_is_negative )
    {
        hud_draw_number_glyph(bitmap_group_index, 12, placement->corner,
                pen_x, pen_y, scale, color, is_interface_bitmap);
    }
}
