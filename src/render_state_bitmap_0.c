/* render_state_bitmap_0 @0x83731868 — draw one input-icon glyph (button/stick prompt) at the current
 * text cursor and advance `bounds->x0` past it. Used by draw_string_and_hack_in_icons.
 *
 * DEVIATION: the DB's applied prototype names these params (bounds, cursor_bounds, color, icon), but
 * the function body (verified via disasm_range(0x83731868, 0x83731868+648)) treats the 3rd GPR (r5) as
 * the icon_hud_element_definition* and the 2nd GPR (r4) as a plain fallback color. There is NO 4th arg:
 * the caller (verified at 0x83732088) reuses r6 as a scratch register while packing the color bytes into
 * r4, so the DB's 4th param (and the earlier reconstruction's unresolved_dead_arg4) is a phantom GPR-slot
 * arg that the callee never reads — dropped. Matches the DEVIATION recorded in draw_string_and_hack_in_icons.c
 * for this same call. Named accordingly here rather than trusting the applied names.
 *
 * DEVIATION: `bitmap_group_index` is computed inline via a raw `global_game_globals->
 * interface_tag_references` lookup at flat index 59 (or, when that block is absent, near-null offset
 * 0xEC) — this is exactly interface_get_tag_index(14)'s own two branches (4*14+3=59, 16*14+0xC=0xEC),
 * so calls that helper directly instead of duplicating its logic. */

#include <stdint.h>
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/bitmap_data.h"
#include "headers/icon_hud_element_definition.h"
#include "headers/icon_flags.h"
#include "headers/interface_tag_index.h"

extern int interface_get_tag_index(int16_t interface_tag_index);
extern uint32_t system_milliseconds(void);
extern void hud_retrieve_bitmap_and_bounding_rect(int bitmap_group_index, int16_t sequence_index, int16_t frame_index, const bitmap_data **bitmap, const real_rectangle2d **clip);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, unsigned __int8 block,
        unsigned __int8 load);
extern int16_t local_player_count(void);
extern float hud_globals_get_scale(uint8_t in_multiplayer);
extern void hud_draw_bitmap_direct(const bitmap_data *bitmap, int16_t placement, const point2d *point, const real_rectangle2d *clip, float scale, float theta, unsigned int color, uint8_t is_interface_bitmap);

void render_state_bitmap_0(rectangle2d *bounds, unsigned int color, const icon_hud_element_definition *icon)
{
    int bitmap_group_index = interface_get_tag_index(_interface_bitmap_iface_map2);

    __int16 frame_index;
    if (icon->frame_rate)
        frame_index = (__int16)(30 * system_milliseconds() / 1000 / icon->frame_rate);
    else
        frame_index = 0;

    const bitmap_data *bitmap = 0;
    const real_rectangle2d *clip = 0;
    hud_retrieve_bitmap_and_bounding_rect(bitmap_group_index, icon->sequence_index, frame_index, &bitmap, &clip);

    if (!bitmap || !_texture_cache_bitmap_get_hardware_format((bitmap_data *)bitmap, 0, 1))
        return;

    __int16 player_count = local_player_count();
    unsigned char in_multiplayer = ((player_count >= 0) + ((unsigned int)player_count <= 1)) & 1;
    float scale = hud_globals_get_scale(in_multiplayer);

    point2d point;
    point.x = (__int16)(bounds->x0 + icon->offset.x * scale + 1.0f);
    point.y = (__int16)(bounds->y1 - icon->offset.y * scale - 2.0f);

    unsigned int effective_color = (icon->flags & (1u << _hud_icon_use_color_bit)) ? icon->color : color;
    hud_draw_bitmap_direct(bitmap, 2, &point, clip, scale, 0.0f, effective_color, 0);

    if (icon->flags & (1u << _hud_icon_absolute_width_bit))
        bounds->x0 = icon->width_offset + point.x;
    else if (clip)
        bounds->x0 = (__int16)((clip->x1 - clip->x0) * bitmap->width + icon->width_offset + point.x);
    else
        bounds->x0 = bitmap->width + icon->width_offset + point.x;
}
