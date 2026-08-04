/* rasterizer_hud_motion_sensor_blip_draw @0x837AA340 — draw one motion-sensor (radar) blip as a textured
 * quad. It ensures the two interface bitmap groups (12 and 13) are resident, and, when radar debug drawing is
 * enabled and the motion-sensor pass has begun, emits a 4-vertex triangle fan centered at the blip position
 * (screen space, scaled by -0.03125) sized by radius*0.0625, tinted by blip_color*fade packed to 0xAARRGGBB
 * (alpha forced opaque), with a full 0..1 UV square.
 *
 * DEVIATION: the DB prototype is (blip_position, fade, radius, blip_color, custom); Hex-Rays kept blip_color
 * (r6) as an unused parameter and invented a phantom `a6` for the color reads. Disasm (0x837AA36C `mr r31,r6`)
 * shows a6 IS blip_color, so the color is read from blip_color->n[0..2]. `custom` is unreferenced in this path.
 * The interleaved float/uint vertex stack layout is reproduced as a flat 24-float buffer with the color slots
 * punned to unsigned int (each vertex = x,y,z,color,u,v; 24-byte stride). */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_rgb_color.h"
#include "headers/d3d_render_boundary.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/interface_tag_index.h"
#include "headers/blam_data_globals.h"


#include "headers/bitmap_data.h"
extern int interface_get_tag_index(int16_t interface_tag_index);
extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern char *tag_get_name(int16_t tag_index);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
                                     const void *vertex_data, unsigned int vertex_stride);

void rasterizer_hud_motion_sensor_blip_draw(const real_point2d *blip_position, float fade, float radius, const real_rgb_color *blip_color, uint8_t custom)
{
    bitmap_group_try_and_get_bitmap(interface_get_tag_index(_interface_bitmap_motion_blip), 0);
    bitmap_group_try_and_get_bitmap(interface_get_tag_index(_interface_bitmap_iface_map1), 0);
    tag_get_name(interface_get_tag_index(_interface_bitmap_motion_blip));
    tag_get_name(interface_get_tag_index(_interface_bitmap_iface_map1));

    if ( !rasterizer_debug_options.draw_hud_motion_sensor )
        return;
    if ( !rasterizer_motion_sensor_begin_said_to_draw )
        return;

    float center_x = (blip_position->n[0] * (float)-0.03125);
    float center_y = (blip_position->n[1] * (float)-0.03125);
    float half = (radius * (float)0.0625);

    float red = ((blip_color->n[0] * fade) * (float)255.0);
    float green = ((blip_color->n[1] * fade) * (float)255.0);
    float blue = ((blip_color->n[2] * fade) * (float)255.0);
    unsigned int packed_color =
        ((((unsigned int)(int)red << 8) & 0xFF00 | 0xFFFF0000 | (unsigned __int8)(int)green) << 8)
        | (unsigned __int8)(int)blue;

    float vertex_buffer[24];
    /* vertex 0 */
    vertex_buffer[0] = center_x - half;
    vertex_buffer[1] = center_y + half;
    vertex_buffer[2] = 0.0f;
    *(unsigned int *)&vertex_buffer[3] = packed_color;
    vertex_buffer[4] = 0.0f;
    vertex_buffer[5] = 0.0f;
    /* vertex 1 */
    vertex_buffer[6] = center_x + half;
    vertex_buffer[7] = center_y + half;
    vertex_buffer[8] = 0.0f;
    *(unsigned int *)&vertex_buffer[9] = packed_color;
    vertex_buffer[10] = 1.0f;
    vertex_buffer[11] = 0.0f;
    /* vertex 2 */
    vertex_buffer[12] = center_x + half;
    vertex_buffer[13] = center_y - half;
    vertex_buffer[14] = 0.0f;
    *(unsigned int *)&vertex_buffer[15] = packed_color;
    vertex_buffer[16] = 1.0f;
    vertex_buffer[17] = 1.0f;
    /* vertex 3 */
    vertex_buffer[18] = center_x - half;
    vertex_buffer[19] = center_y - half;
    vertex_buffer[20] = 0.0f;
    *(unsigned int *)&vertex_buffer[21] = packed_color;
    vertex_buffer[22] = 0.0f;
    vertex_buffer[23] = 1.0f;

    D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4u, vertex_buffer, 0x18u);
}
