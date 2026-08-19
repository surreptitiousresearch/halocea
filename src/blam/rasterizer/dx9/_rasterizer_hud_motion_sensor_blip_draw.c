/* _rasterizer_hud_motion_sensor_blip_draw @0x837AA340 — draw one motion-sensor (radar) blip quad into
 * the accumulating blip render target, between _rasterizer_hud_motion_sensor_blip_begin and
 * _rasterizer_hud_motion_sensor_blip_end. Only draws when radar debug drawing is on and blip_begin
 * actually started the pass (rasterizer_motion_sensor_begin_said_to_draw). The blip is a screen-aligned
 * quad centred at blip_position (scaled into the target's -1/32 units) sized by radius, tinted by
 * blip_color modulated by fade, and rasterized as a triangle fan of four dynamic_screen_vertex
 * (position, packed ARGB, texcoord — 24 bytes, the 0x18 stride handed to D3DDevice_DrawVerticesUP).
 *
 * The interface_get_tag_index / bitmap_group_try_and_get_bitmap / tag_get_name calls up front mirror the
 * begin/end siblings — they touch the motion-blip (12) and iface-map1 (13) interface bitmaps so the tag
 * references stay resident; their results are unused here.
 *
 * ABI: the registered prototype's 4th parameter is the color pointer (r6) and the 5th (custom, r7) is
 * unused by the body; the decompiler mis-split the color pointer into an unused `blip_color` plus a
 * trailing `a6` — disasm confirms r6 is the single real color pointer, read as n[0]=red, n[1]=green,
 * n[2]=blue. */

#include <stdint.h>
#include "headers/interface_tag_index.h"
#include "headers/real_point2d.h"
#include "headers/real_rgb_color.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/D3DDevice.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"


#include "headers/bitmap_data.h"
extern int interface_get_tag_index(int16_t interface_tag_index);
extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern char *tag_get_name(int tag_index);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
                                     const void *vertex_data, unsigned int vertex_stride);

void _rasterizer_hud_motion_sensor_blip_draw(const real_point2d *blip_position, float fade, float radius,
                                             const real_rgb_color *blip_color, uint8_t custom)
{
    /* Keep the motion-blip (12) and iface-map1 (13) interface bitmap tags referenced (results unused). */
    bitmap_group_try_and_get_bitmap(interface_get_tag_index(_interface_bitmap_motion_blip), 0);
    bitmap_group_try_and_get_bitmap(interface_get_tag_index(_interface_bitmap_iface_map1), 0);
    tag_get_name(interface_get_tag_index(_interface_bitmap_motion_blip));
    tag_get_name(interface_get_tag_index(_interface_bitmap_iface_map1));

    if ( !rasterizer_debug_options.draw_hud_motion_sensor )
        return;
    if ( !rasterizer_motion_sensor_begin_said_to_draw )
        return;

    /* Blip centre in target units (1 texel = -1/32) and half-extent (radius * 1/16). */
    float center_x = (blip_position->n[0] * -0.03125f);
    float center_y = (blip_position->n[1] * -0.03125f);
    float half = (radius * 0.0625f);

    /* Tint: blip_color modulated by fade, scaled to 0..255 and packed as ARGB with opaque alpha. */
    float blip_red   = ((blip_color->n[0] * fade) * 255.0f);
    float blip_green = ((blip_color->n[1] * fade) * 255.0f);
    float blip_blue  = ((blip_color->n[2] * fade) * 255.0f);
    unsigned int packed_color =
        ((((unsigned int)(int64_t)blip_red << 8) & 0xFF00 | 0xFFFF0000 | (uint8_t)(int64_t)blip_green) << 8)
        | (uint8_t)(int64_t)blip_blue;

    float left   = (center_x - half);
    float right   = (center_x + half);
    float top    = (center_y + half);
    float bottom = (center_y - half);

    /* Vertex stream, four dynamic_screen_vertex. DEVIATION: the decompiler renders this frame slice as a
     * flat float array with the packed colour punned through a float slot; the stores at
     * 0x837AA430-0x837AA4E4 are a 24-byte stride off `addi r6,r1,var_90` (0x837AA448) with the colour at
     * +0x0C of each — i.e. the rasterizer's dynamic_screen_vertex, verified store-by-store. */
    dynamic_screen_vertex vertices[4];
    vertices[0].position.n[0] = left;
    vertices[0].position.n[1] = top;
    vertices[0].position.n[2] = 0.0f;
    vertices[0].color = packed_color;
    vertices[0].texcoord.n[0] = 0.0f;
    vertices[0].texcoord.n[1] = 0.0f;
    vertices[1].position.n[0] = right;
    vertices[1].position.n[1] = top;
    vertices[1].position.n[2] = 0.0f;
    vertices[1].color = packed_color;
    vertices[1].texcoord.n[0] = 1.0f;
    vertices[1].texcoord.n[1] = 0.0f;
    vertices[2].position.n[0] = right;
    vertices[2].position.n[1] = bottom;
    vertices[2].position.n[2] = 0.0f;
    vertices[2].color = packed_color;
    vertices[2].texcoord.n[0] = 1.0f;
    vertices[2].texcoord.n[1] = 1.0f;
    vertices[3].position.n[0] = left;
    vertices[3].position.n[1] = bottom;
    vertices[3].position.n[2] = 0.0f;
    vertices[3].color = packed_color;
    vertices[3].texcoord.n[0] = 0.0f;
    vertices[3].texcoord.n[1] = 1.0f;

    D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, vertices, 0x18);
}
