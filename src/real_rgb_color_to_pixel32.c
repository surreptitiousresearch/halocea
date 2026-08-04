/* real_rgb_color_to_pixel32 @0x8369AE90 — pack a normalized RGB color into a 32-bit pixel with a
 * zero alpha byte. Each channel is scaled by 255 and truncated to a byte:
 *   red -> bits 23..16, green -> 15..8, blue -> 7..0. */

#include "headers/real_rgb_color.h"

int real_rgb_color_to_pixel32(const real_rgb_color *color)
{
    unsigned int r = (unsigned char)(color->n[0] * 255.0f);
    unsigned int g = (unsigned char)(color->n[1] * 255.0f);
    unsigned int b = (unsigned char)(color->n[2] * 255.0f);
    return (r << 16) | (g << 8) | b;
}
