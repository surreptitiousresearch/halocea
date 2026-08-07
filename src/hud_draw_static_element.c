/* hud_draw_static_element @0x837A0358 — draw a static (non-numeric) HUD icon: resolves its bitmap and
 * sprite clip rect from the sequence table, resolves flat/disabled/flash color, then draws the icon
 * itself followed by each of its multitexture overlay layers at the same anchor point.
 *
 * DEVIATION: call-site argument mapping for `hud_draw_bitmap_with_meter`/`hud_calculate_point` was
 * cross-checked against disasm_range(0x837A0358, 0x837A05D0) rather than trusted from the (heavily
 * scrambled) decompiler arg ordering, using the same dead-GPR-shadow rule established for those two
 * functions' own reconstructions.
 *
 * `is_interface_bitmap` here is `bitmap_group->type == 4` (bitmap_group.h's `type` enum — value 4 is
 * the "Interface Bitmap" usage) — what the binary stores at r1+0x5F @0x837A0448, i.e. param 10.
 *
 * DEVIATION: there is no uninitialized slot at this call site. hud_draw_bitmap_with_meter takes TEN
 * parameters and this call writes exactly the two stack bytes it has — r1+0x57 = (draw_flags >> 2) & 1
 * @0x837A045C and r1+0x5F = the interface-bitmap flag @0x837A0448; r1+0x60 is this function's own
 * `point` local (@0x837A0548), which settles the arity. */

#include <stdint.h>
#include "headers/hud_absolute_placement_definition.h"
#include "headers/hud_scaling_flags.h"
#include "headers/hud_draw_flags.h"
#include "headers/static_hud_element_definition.h"
#include "headers/multitexture_overlay_hud_element_definition.h"
#include "headers/bitmap_data.h"
#include "headers/real_rectangle2d.h"
#include "headers/real_vector2d.h"
#include "headers/point2d.h"
#include "headers/render_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

typedef struct rasterizer_meter_parameters rasterizer_meter_parameters;
typedef struct multitexture_overlay_hud_element_definition multitexture_overlay_hud_element_definition;


#include "headers/hud_placement_definition.h"
#include "headers/hud_placement_definition.h"
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern const real_rectangle2d *get_sprite_clip_rect(int bitmap_group_index, int16_t sequence_index, int16_t frame);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block,
        uint8_t load);
extern unsigned int get_flash_color(const hud_color_definition *hud_color_def, int reference_value);

extern void hud_calculate_point(int16_t local_player_index, const hud_absolute_placement_definition *absolute_placement, const hud_placement_definition *placement, const bitmap_data *bitmap, uint8_t in_multiplayer, float override_scale, point2d *result);

extern void hud_calculate_bitmap_bounds(const bitmap_data *bitmap, int16_t placement_type, const real_rectangle2d *clip, real_rectangle2d *bounds, uint8_t is_interface_bitmap);

extern void hud_draw_bitmap_with_meter(rasterizer_meter_parameters *meter_parameters, const bitmap_data *bitmap, const hud_absolute_placement_definition *absolute_placement, const hud_placement_definition *placement, const real_rectangle2d *clip, float scale, float theta, unsigned int color32, uint8_t in_multiplayer, uint8_t is_interface_bitmap);

extern void hud_draw_multitexture_overlay(const multitexture_overlay_hud_element_definition *overlay,
        int16_t local_player_index, const point2d *point, const real_rectangle2d *clip,
        const real_rectangle2d *bounds, const real_vector2d *xy_scale, float theta, unsigned int color);

void hud_draw_static_element(int16_t local_player_index, const hud_absolute_placement_definition *absolute_placement, const static_hud_element_definition *static_element, int16_t draw_flags, int flash_reference_time)
{
    int bitmap_group_index = static_element->interface_bitmap.index;
    uint16_t *bitmap_group_header = TAG_GET(uint16_t, bitmap_group_index);

    bitmap_data *bitmap = bitmap_group_get_bitmap_from_sequence(bitmap_group_index, static_element->sequence_index, 0);
    if (!_texture_cache_bitmap_get_hardware_format(bitmap, 0, 1))
        return;

    const real_rectangle2d *clip = get_sprite_clip_rect(bitmap_group_index, static_element->sequence_index, 0);

    unsigned int color;
    if (draw_flags & (1u << _hud_draw_disabled_bit))
        color = static_element->colors.disabled_color;
    else if (draw_flags & (1u << _hud_draw_flashing_bit))
        color = get_flash_color(&static_element->colors, flash_reference_time);
    else
        color = static_element->colors.color;

    int is_interface_bitmap = (*bitmap_group_header == 4);

    float scale = hcex_hud_globals_scale;
    if (static_element->placement.multiplayer_scaling_flags & (1u << _hud_use_high_resolution_scale_bit))
        scale = hcex_hud_globals_scale * 0.5f;

    hud_draw_bitmap_with_meter(0, bitmap, absolute_placement, &static_element->placement, clip, scale, 0.0f, color,
                                (draw_flags & (1u << _hud_draw_in_multiplayer_bit)) != 0, is_interface_bitmap);

    int overlay_count = static_element->multitexture_overlays.count;
    if (overlay_count <= 0)
        return;

    real_rectangle2d default_clip;
    default_clip.__s1.x0 = 0.0f;
    default_clip.__s1.x1 = 1.0f;
    default_clip.__s1.y0 = 0.0f;
    default_clip.__s1.y1 = 1.0f;

    for (int i = 0; i < overlay_count; ++i)
    {
        const multitexture_overlay_hud_element_definition *overlay =
                &((const multitexture_overlay_hud_element_definition *)
                  static_element->multitexture_overlays.address)[i];

        if (is_interface_bitmap)
        {
            default_clip.__s1.x1 = (float)bitmap->width;
            default_clip.__s1.y1 = (float)bitmap->height;
        }

        if (!clip)
            clip = &default_clip;

        real_vector2d xy_scale;
        xy_scale.__s1.i = static_element->placement.scale.__s1.i * scale;
        xy_scale.__s1.j = static_element->placement.scale.__s1.j * scale;

        unsigned char use_multiplayer_scaling;
        if ((draw_flags & (1u << _hud_draw_in_multiplayer_bit)) && !(static_element->placement.multiplayer_scaling_flags & (1u << _hud_dont_scale_offset_bit)))
            use_multiplayer_scaling = 1;
        else
            use_multiplayer_scaling = 0;

        point2d point;
        hud_calculate_point(render.local_player_index, absolute_placement, &static_element->placement, 0,
                             use_multiplayer_scaling, 0.0f, &point);

        real_rectangle2d bounds;
        hud_calculate_bitmap_bounds(bitmap, absolute_placement->corner, clip, &bounds, is_interface_bitmap);

        hud_draw_multitexture_overlay(overlay, local_player_index, &point, clip, &bounds, &xy_scale, 0.0f, color);
    }
}
