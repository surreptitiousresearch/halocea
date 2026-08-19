/* trouble_is_brewing_render @0x83732C28 — draw the "trouble is brewing" network-trouble icon in the
 * lower-right of the screen while the trouble timer is active. Loads the icon bitmap on demand; if the
 * bitmap tag is unset or the bitmap cannot be fetched, falls back to a translucent red quad in its place.
 *
 * DEVIATION: the decompiler renders the icon rectangle as a packed 64-bit constant
 * (0x196023601D60276); it is {y0=406, x0=566, y1=470, x1=630}. */

#include <stdint.h>
#include <string.h>
#include "headers/rectangle2d.h"
#include "headers/bitmap_data.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/blam_data_globals.h"


extern void trouble_is_brewing_load_icon(uint8_t do_load);
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern void draw_quad(rectangle2d *rect, unsigned int color);
extern void draw_bitmap_in_rect(bitmap_data *bitmap, rectangle2d *rect, rectangle2d *bitmap_rect, rectangle2d *clip_rect, unsigned int argb, rasterizer_dynamic_screen_geometry_parameters *multitexture_params, uint8_t no_plasma);

void trouble_is_brewing_render(void)
{
    if (trouble_is_brewing_time == -1)
        return;

    rectangle2d rect = { 406, 566, 470, 630 };
    trouble_is_brewing_load_icon(0);

    bitmap_data *icon;
    if (trouble_is_brewing_bitmap_tag == -1
        || (icon = bitmap_group_get_bitmap_from_sequence(trouble_is_brewing_bitmap_tag, 0, 0)) == nullptr)
    {
        draw_quad(&rect, 0x80FF0000);
    }
    else
    {
        rasterizer_dynamic_screen_geometry_parameters multitexture_params;
        memset(&multitexture_params, 0, sizeof(multitexture_params));
        draw_bitmap_in_rect(icon, &rect, nullptr, nullptr, 0xFFFFFFFF, &multitexture_params, 0);
    }
}
