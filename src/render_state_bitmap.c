/* render_state_bitmap @0x836A3C70 — draw one HUD messaging-icon glyph (from hud_globals->messaging.
 * messaging_icons) at the text cursor held in `bounds` (x0/y1 corner), then advance bounds->__s1.x0 past the
 * glyph. The frame index animates at game_time / icon->frame_rate; the glyph is scaled by the global
 * hcex_render_state_bitmap_scale, shrunk a further 0.75x in >1-local-player splitscreen (unless HCEX
 * coop assigns a single local player). Icon flag 0x2 substitutes the icon's own color for the caller's;
 * flag 0x4 advances by the scaled width_offset alone, otherwise by the scaled bitmap width (clipped by
 * the sprite's bounding rect when one exists) plus width_offset. Near-twin of render_state_bitmap_0
 * (the input-button-glyph variant), without its +1/-2 pixel nudges and with a scaled advance.
 *
 * DEVIATION: reg-alloc-failure decompile rebuilt from disasm (0x836A3C70-3F64). The DB's applied
 * prototype names the params (bounds, cursor_bounds, color, icon), but the registers say r4 is the
 * fallback color and r5 the icon; r6 is never referenced — the same stale-name arrangement already
 * recorded for render_state_bitmap_0, named accordingly. hud_draw_bitmap_direct's float scale/theta
 * args burn the r7/r8 GPR shadows, so color/is_interface_bitmap really travel in r9/r10. */

#include <stdint.h>
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/bitmap_data.h"
#include "headers/icon_hud_element_definition.h"
#include "headers/hud_globals.h"
#include "headers/icon_flags.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern void hud_retrieve_bitmap_and_bounding_rect(int bitmap_group_index, int16_t sequence_index, int16_t frame_index, const bitmap_data **bitmap, const real_rectangle2d **clip);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, unsigned __int8 block,
        unsigned __int8 load);
extern int16_t local_player_count(void);
extern void hud_draw_bitmap_direct(const bitmap_data *bitmap, int16_t placement, const point2d *point, const real_rectangle2d *clip, float scale, float theta, unsigned int color, uint8_t is_interface_bitmap);

void render_state_bitmap(rectangle2d *bounds, unsigned int color, const icon_hud_element_definition *icon,
        unsigned int unresolved_dead_arg4)
{
    int bitmap_group_index = hud_globals->messaging.messaging_icons.index;

    __int16 frame_index;
    if ( icon->frame_rate )
        frame_index = (__int16)(game_time_get() / icon->frame_rate);
    else
        frame_index = 0;

    const bitmap_data *bitmap = 0;
    const real_rectangle2d *clip = 0;
    hud_retrieve_bitmap_and_bounding_rect(bitmap_group_index, icon->sequence_index, frame_index,
            &bitmap, &clip);

    if ( !bitmap || !_texture_cache_bitmap_get_hardware_format((bitmap_data *)bitmap, 0, 1) )
        return;

    unsigned __int8 splitscreen_shrink = local_player_count() > 1 && hcex_coop_local_player_index < 0;
    float shrink = splitscreen_shrink ? 0.75f : 1.0f;
    float scale = shrink * hcex_render_state_bitmap_scale;

    point2d point;
    point.__s1.x = (__int16)(int)((float)icon->offset.__s1.x * scale + (float)bounds->__s1.x0);
    point.__s1.y = (__int16)(int)-((float)icon->offset.__s1.y * scale - (float)bounds->__s1.y1);

    unsigned int effective_color = (icon->flags & (1u << _hud_icon_use_color_bit)) ? icon->color : color;
    hud_draw_bitmap_direct(bitmap, 2, &point, clip, scale, 0.0f, effective_color, 0);

    if ( icon->flags & (1u << _hud_icon_absolute_width_bit) )
        bounds->__s1.x0 = (__int16)(int)((float)icon->width_offset * scale + (float)point.__s1.x);
    else if ( clip )
        bounds->__s1.x0 = (__int16)(int)(((clip->__s1.x1 - clip->__s1.x0) * (float)bitmap->width
                + (float)icon->width_offset) * scale + (float)point.__s1.x);
    else
        bounds->__s1.x0 = (__int16)(int)((float)(bitmap->width + icon->width_offset) * scale + (float)point.__s1.x);
}
