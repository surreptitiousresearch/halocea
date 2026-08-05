/* interface_get_rgb_color @0x836A75B0 — look up a real (float) ARGB interface color and quantize each of
 * its four channels to 16-bit fixed point (x*65535), storing the packed argb_color in `color`.
 *
 * Deviation from decompiler: its output aliased the temp's storage into the result and read one channel
 * back through a re-store (a scheduling artifact). The disassembly computes the same per-channel
 * quantization for all four channels (n[0]=alpha .. n[3]=blue); reproduced directly. */

#include <stdint.h>
#include "headers/argb_color.h"
#include "headers/real_argb_color.h"

extern real_argb_color *interface_get_real_argb_color(int16_t interface_color_table_index, int16_t color_index, real_argb_color *color);

argb_color * interface_get_rgb_color(int16_t interface_color_table_index, int16_t color_index, argb_color *color)
{
    real_argb_color real_color;
    interface_get_real_argb_color(interface_color_table_index, color_index, &real_color);

    color->n[0] = (uint16_t)(int64_t)(real_color.n[0] * (float)65535.0);
    color->n[1] = (uint16_t)(int64_t)(real_color.n[1] * (float)65535.0);
    color->n[2] = (uint16_t)(int64_t)(real_color.n[2] * (float)65535.0);
    color->n[3] = (uint16_t)(int64_t)(real_color.n[3] * (float)65535.0);
    return color;
}
